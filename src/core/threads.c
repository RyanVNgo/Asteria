#include "threads.h"
#include "glib.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdatomic.h>

/**************************************************/
/* Task and Task Queue implementation */

typedef struct Task {
  void (*function)(void* arg);
  void* arg;
  struct Task* next;
} Task;

typedef struct TaskQueue {
  Task* front;
  Task* back;
  int count;
} TaskQueue;

void enqueue_task(TaskQueue* task_queue, Task* new_task) {
  if (!task_queue->back) {
    task_queue->front = new_task;
  } else {
    task_queue->back->next = new_task;
  }
  task_queue->back = new_task;
  task_queue->count++;
  return;
}

Task* dequeue_task(TaskQueue* task_queue) {
  if (!task_queue->front) return NULL;

  Task* task = task_queue->front;

  task_queue->front = task_queue->front->next;
  if (!task_queue->front) {
    task_queue->back = NULL;
  }

  task_queue->count--;
  return task;
}

TaskQueue* task_queue_init() {
  TaskQueue* new_task_queue = malloc(sizeof(TaskQueue));
  new_task_queue->front = NULL;
  new_task_queue->back = NULL;
  new_task_queue->count = 0;
  return new_task_queue;
}

void destroy_task_queue(TaskQueue* task_queue) {
  Task* task;
  while (task_queue->front && task_queue->count > 0) {
    task = dequeue_task(task_queue);
    free(task);
  }
  free(task_queue);
  return;
}

/**************************************************/
/* Thread pool implementation */

typedef struct ThreadPool {
  pthread_t* threads;
  TaskQueue* task_queue;
  pthread_mutex_t queue_mutex;
  pthread_cond_t queue_cond;
  int max_threads;
  int stop;
} ThreadPool;

void* worker_thread(void* arg) {
  ThreadPool* pool = (ThreadPool*)arg;

  while (1) {
    pthread_mutex_lock(&pool->queue_mutex);

    while (pool->task_queue->count == 0 && !pool->stop) {
      pthread_cond_wait(&pool->queue_cond, &pool->queue_mutex);
    }

    if (pool->stop) {
      pthread_mutex_unlock(&pool->queue_mutex);
      break;
    }

    Task* fetched_task = dequeue_task(pool->task_queue);
    pthread_mutex_unlock(&pool->queue_mutex);

    if (fetched_task) {
      fetched_task->function(fetched_task->arg);
      free(fetched_task);
    }
  }

  pthread_exit(NULL);
  return NULL;
}

void submit_task(ThreadPool* pool, void (*function)(void*), void* arg) {
  Task* new_task = (Task*)malloc(sizeof(Task));
  if (!new_task) return;

  new_task->function = function;
  new_task->arg = arg;
  new_task->next = NULL;

  pthread_mutex_lock(&pool->queue_mutex);
  enqueue_task(pool->task_queue, new_task);
  pthread_cond_signal(&pool->queue_cond);
  pthread_mutex_unlock(&pool->queue_mutex);
  return;
}

ThreadPool* thread_pool_init(int max_threads) {
  ThreadPool* pool = malloc(sizeof(ThreadPool));
  pool->threads = malloc(max_threads * sizeof(pthread_t));
  pool->task_queue = task_queue_init();
  pthread_mutex_init(&pool->queue_mutex, NULL);
  pthread_cond_init(&pool->queue_cond, NULL);
  pool->max_threads = max_threads;
  pool->stop = 0;

  for (int i = 0; i < max_threads; i++) {
    pthread_create(&pool->threads[i], NULL, worker_thread, (void*)pool);
    g_print("Thread %d created. ", i);
  }
  g_print("\n\n");

  return pool;
}

void thread_pool_shutdown(ThreadPool* pool) {
  g_print("\nShutting down thread pool...\n");

  pthread_mutex_lock(&pool->queue_mutex);
  pool->stop = 1;
  pthread_cond_broadcast(&pool->queue_cond);
  pthread_mutex_unlock(&pool->queue_mutex);

  int i = 0;
  for (; i < pool->max_threads; i++) {
    pthread_join(pool->threads[i], NULL);
    g_print("Thread joined...\n");
  }

  g_print("%d threads joined.\n", i);
  g_print("Destorying thread pool...\n");

  free(pool->threads);
  destroy_task_queue(pool->task_queue);
  pthread_mutex_destroy(&pool->queue_mutex);
  pthread_cond_destroy(&pool->queue_cond);

  g_print("Destroyed thread pool.\n");
  return;
}

/**************************************************/
/* Thread monitor implementation */

typedef struct ThreadMonitor {
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  atomic_int counter;
  int thread_count;
} ThreadMonitor;

ThreadMonitor* thread_monitor_init(int thread_count) {
  ThreadMonitor* new_thread_monitor = malloc(sizeof(ThreadMonitor));
  pthread_mutex_init(&new_thread_monitor->mutex, NULL);
  pthread_cond_init(&new_thread_monitor->cond, NULL);
  new_thread_monitor->counter = 0;
  new_thread_monitor->thread_count = thread_count;

  return new_thread_monitor;
}

void thread_monitor_signal(ThreadMonitor* thread_monitor) {
  pthread_mutex_lock(&thread_monitor->mutex);
  thread_monitor->counter++;
  pthread_cond_signal(&thread_monitor->cond);
  pthread_mutex_unlock(&thread_monitor->mutex);
}

void thread_monitor_wait(ThreadMonitor* thread_monitor) {
  pthread_mutex_lock(&thread_monitor->mutex);
  while (thread_monitor->counter < thread_monitor->thread_count) {
    pthread_cond_wait(&thread_monitor->cond, &thread_monitor->mutex);
  }
  pthread_mutex_unlock(&thread_monitor->mutex);
}

void thread_monitor_destroy(ThreadMonitor* thread_monitor) {
  pthread_mutex_destroy(&thread_monitor->mutex);
  pthread_cond_destroy(&thread_monitor->cond);
  free(thread_monitor);
  return;
}

