#ifndef THREADS_H
#define THREADS_H

/* external libraries */
#include <pthread.h>

typedef struct TaskQueue TaskQueue;
typedef struct ThreadPool ThreadPool;
typedef struct ThreadMonitor ThreadMonitor;

ThreadPool* thread_pool_init(int max_threads);

void submit_task(ThreadPool* pool, void (*function)(void*), void* arg);
void thread_pool_shutdown(ThreadPool* pool);

ThreadMonitor* thread_monitor_init(int num_threads);
void thread_monitor_signal(ThreadMonitor* thread_monitor);
void thread_monitor_wait(ThreadMonitor* thread_monitor);
void thread_monitor_destroy(ThreadMonitor* thread_monitor);



#endif
