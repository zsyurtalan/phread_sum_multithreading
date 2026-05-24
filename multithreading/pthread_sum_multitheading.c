#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#define num_threads 5

int global_sum=0;
int random_num;
pthread_mutex_t mutex;
void *runner(void *param);

int main() {
    srand(time(NULL));
    int min=10000,max=50000;
    random_num=(rand()%(max-min+1))+min;
    printf("random num:%d\n",random_num);
    pthread_t threads[num_threads];
    int ids[num_threads];
    int rc;
    rc=pthread_mutex_init(&mutex,NULL);
    if(rc!=0){
        fprintf(stderr,"pthread_mutex_init failed:%s\n", strerror(rc));
        exit(EXIT_FAILURE);
    }
    int i;
    for(i=0; i<num_threads;i++){
        ids[i]=i;
        rc=pthread_create(&threads[i],NULL,runner,&ids[i]);
        if(rc!=0){
            fprintf(stderr,"Error creating thread %d:%s\n",i,strerror(rc));
            pthread_mutex_destroy(&mutex);
            exit(EXIT_FAILURE);
        }
    }
    for(i=0;i<num_threads;i++){
        rc=pthread_join(threads[i],NULL);
        if(rc!=0){
            fprintf(stderr,"Error joining thread %d:%s\n",i,strerror(rc));
            pthread_mutex_destroy(&mutex);
            exit(EXIT_FAILURE);
        }
    }
    printf("global sum=%d\n",global_sum);
    rc=pthread_mutex_destroy(&mutex);
    if(rc!=0){
        fprintf(stderr,"pthread_mutex_destroy failed:%s\n", strerror(rc));
        exit(EXIT_FAILURE);
    }
    return 0;
}

void *runner(void *param){
    int id=*(int*)param;
    int sum=0;
    int rc;
    for(int i=id+1;i<=random_num;i+=num_threads){
        sum+=i;
    }
    printf("thread %d partial sum= %d\n",id,sum);
    rc=pthread_mutex_lock(&mutex);
    if(rc!=0){
        fprintf(stderr,"pthread_mutex_lock failed in thread %d: %s\n",id,strerror(rc));
        pthread_exit(NULL);
    }
    global_sum+=sum;
    rc=pthread_mutex_unlock(&mutex);
    if(rc!=0) {
        fprintf(stderr,"pthread_mutex_unlock failed in thread %d:%s\n", id, strerror(rc));
        pthread_exit(NULL);
    }
    pthread_exit(NULL);
}
//each thread computes a partial sum of numbers 
//when the partial sum is calculated,it is added to the global_sum variables
//since multiple thrads access and modify global_sum,this section becomes a critical section 
//and must be protected using mutex to avoid race conditions
//all system calls such as pthread_create, pthread_join, and mutex operations
//are checked for errors and appropriate actions are taken if a failure occurs