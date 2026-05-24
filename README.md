# Multithreaded Sum Computation using Pthreads

This project demonstrates parallel computation using POSIX threads (pthreads) in C.
---
## Problem Description

A main thread generates a random number between **10,000 and 50,000**.
Then the program creates **5 threads**, each responsible for computing a partial sum of numbers.
---
## Thread Work Distribution

Each thread `i` computes:

* Thread 0 → 1, 6, 11, 16, ...
* Thread 1 → 2, 7, 12, 17, ...
* Thread 2 → 3, 8, 13, 18, ...
* Thread 3 → 4, 9, 14, 19, ...
* Thread 4 → 5, 10, 15, 20, ...

This ensures **balanced workload distribution** across threads.
---
## Synchronization

Since all threads update a shared variable (`global_sum`), a **race condition** may occur.

To prevent this:

* A **mutex lock (`pthread_mutex_t`)** is used
* Only one thread can update `global_sum` at a time
* Other threads wait for the critical section to be free
---
## Race Condition Explanation

A race condition occurs when multiple threads:

* read and write shared data simultaneously
* produce inconsistent results

This is prevented using:

* `pthread_mutex_lock`
* `pthread_mutex_unlock`
---
## Error Handling

All system calls are checked:

* pthread_create
* pthread_join
* pthread_mutex_init
* pthread_mutex_lock
* pthread_mutex_unlock
* pthread_mutex_destroy

If any fail, the program prints an error message and exits safely.
---
## Compilation

```bash
gcc main.c -o program -pthread
```
## Execution

```bash
./program
```
## Concepts Demonstrated

* Multithreading (POSIX threads)
* Load balancing
* Race conditions
* Mutex synchronization
* Critical section protection
* Error handling in system programming
* 
## Author

zsyurtalan
