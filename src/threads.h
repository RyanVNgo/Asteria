#ifndef THREADS_H
#define THREADS_H

#include <pthread.h>

typedef struct TaskQueue TaskQueue;
typedef struct ThreadPool ThreadPool;

ThreadPool* thread_pool_init(int maxThreads);
void submit_task(ThreadPool* pool, void (*function)(void*), void* arg);
void thread_pool_shutdown(ThreadPool* pool);



#endif
