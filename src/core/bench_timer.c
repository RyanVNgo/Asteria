/* primary header include */
#include "bench_timer.h"

/* external libraries */
#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>

typedef struct BenchTimer {
  char* timer_name;
  float start;
  float interval;
} BenchTimer;

void bench_timer_start(BenchTimer** bench_timer, char* timer_name) {
  *bench_timer = malloc(sizeof(BenchTimer));
  (*bench_timer)->start = 0;
  (*bench_timer)->interval = 0;

  struct timespec start;
  clock_gettime(CLOCK_MONOTONIC, &start);
  (*bench_timer)->start = (1000 * start.tv_sec) + (start.tv_nsec / 1000000.0);
  (*bench_timer)->interval = (*bench_timer)->start;

  if (timer_name) (*bench_timer)->timer_name = timer_name;
  else (*bench_timer)->timer_name = "TIMER";

  g_print("Bench Timer - %s -> START\n", (*bench_timer)->timer_name);

  return;
}

void bench_timer_interval(BenchTimer* bench_timer, char* interval_name) {
  float prev_time = bench_timer->interval;

  struct timespec interval;
  clock_gettime(CLOCK_MONOTONIC, &interval);
  float curr_time = (1000 * interval.tv_sec) + (interval.tv_nsec / 1000000.0);

  bench_timer->interval = curr_time;

  double elapsed_time = curr_time - prev_time;

  if (!interval_name) interval_name = "INTERVAL";

  g_print("Bench Timer - %s - %16s -> %7.1f milliseconds\n", bench_timer->timer_name, interval_name, elapsed_time);

  return;
}

void bench_timer_finish(BenchTimer* bench_timer) {
  struct timespec finish;
  clock_gettime(CLOCK_MONOTONIC, &finish);
  float finish_time = (1000 * finish.tv_sec) + (finish.tv_nsec / 1000000.0);

  double total_time = finish_time - bench_timer->start;

  g_print("Bench Timer - %s -> FINISHED\n", bench_timer->timer_name);
  g_print("Bench Timer - %s - TOTAL TIME -> %7.1f milliseconds\n", bench_timer->timer_name, total_time);

  free(bench_timer);
}
