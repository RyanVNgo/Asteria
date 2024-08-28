#ifndef BENCH_TIMER_H
#define BENCH_TIMER_H

typedef struct BenchTimer BenchTimer;

/*
 * Example usage of BenchTimer
 *
 * void foo() {
 *   BenchTimer* bench_timer;
 *   bench_timer_start(bench_timer, "Foo test");
 *
 *   ** some intensive process 1 **
 *   
 *   bench_timer_interval(bench_timer, "Process 1");
 *
 *   ** some intensive process 2 **
 *
 *   bench_timer_interval(bench_timer, "Process 2");
 *
 *   ** some intensive process 3 **
 *
 *   bench_timer_interval(bench_timer, "Process 3");
 *   bench_timer_finish(bench_timer);
 * }
 *
 * Output:
 * Bench Timer - Foo Test -> START
 * Bench Timer - Foo Test - Process 1 -> 243.6 milliseconds
 * Bench Timer - Foo Test - Process 2 -> 192.1 milliseconds
 * Bench Timer - Foo Test - Process 3 -> 398.3 milliseconds
 * Bench Timer - Foo Test -> FINISHED
 * Bench Timer - Foo Test - TOTAL TIME -> 834.0 milliseconds
 *
 */

/* 
 * bench_timer_start() both initializes the bench_timer
 * and starts the timer.
 */
void bench_timer_start(BenchTimer** bench_timer, char* timer_name);

/*
 * Allows for timing several processes without
 * needing to make new BenchTimers.
 * Time is based on interval between most recent
 * bench_timer_interval() or bench_timer_start().
 */
void bench_timer_interval(BenchTimer* bench_timer, char* interval_name);

/*
 * Displays interval time between start and finish
 * along with interval between most recent bench_timer_interval()
 * and finish (if applicable)
 */
void bench_timer_finish(BenchTimer* bench_timer);

#endif

