/* primary header include */
#include "image_operations.h"

/* external libraries */
#include <math.h>

/* START opaque functions for data scaling */
void square_root_uint16_func(uint16_t** img_data, int pixel_count) {
  int i = 0;
  for (; i < pixel_count; i++) {
    (*img_data)[i] = sqrt((*img_data)[i] / (float)UINT16_MAX) * UINT16_MAX;
  }

  return;
}

void autostretch_uint16_thread_func(void* args_in) {
  struct func_args {
    ThreadMonitor* thread_monitor;
    uint16_t* img_data;
    int pixel_count;
    float mtf_val;
  };
  struct func_args* args = (struct func_args*)args_in;

  float x = 0;
  for (int i = 0; i < args->pixel_count; i++) {
    x = args->img_data[i] / (float)UINT16_MAX;
    args->img_data[i] = UINT16_MAX * ((args->mtf_val - 1) * x) / (((2 * args->mtf_val - 1) * x) - args->mtf_val);
  }

  thread_monitor_signal(args->thread_monitor);
  return;
}

void autostretch_uint16_func(ThreadPool* thread_pool, uint16_t** fits_data, int pixel_count, int dim_count) {
  int channel_size = pixel_count / dim_count;
  unsigned long long dim_avg[dim_count];
  float mtf_val[dim_count];
  const float t_val = 0.2;

  /* find pixl avg per channel and pow val */
  int i = 0;
  int c = 0;
  for (c = 0; c < dim_count; c++) {
    dim_avg[c] = 0;
    i = 0;
    for(; i < channel_size; i++) {
      dim_avg[c] += *(*fits_data + i + (channel_size * c));
    }
    dim_avg[c] /= channel_size;
    float n_dim_avg = dim_avg[c] / (float)UINT16_MAX;
    mtf_val[c] = (-n_dim_avg + t_val * n_dim_avg) / (2 * t_val * n_dim_avg - t_val - n_dim_avg);
  }

  /* define arg struct */
  struct func_args {
    ThreadMonitor* thread_monitor;
    uint16_t* img_data;
    int pixel_count;
    float mtf_val;
  };
  
  /* make thread monitor */
  int task_count = 3;
  ThreadMonitor* thread_monitor = thread_monitor_init(task_count);

  /* submit tasks */
  struct func_args* task_args[task_count];
  for (int t = 0; t < task_count; t++) {
    task_args[t] = malloc(sizeof(struct func_args));
    task_args[t]->thread_monitor = thread_monitor;
    task_args[t]->img_data = *fits_data + (channel_size * t);
    task_args[t]->pixel_count = channel_size;
    task_args[t]->mtf_val= mtf_val[t];
    submit_task(thread_pool, autostretch_uint16_thread_func, task_args[t]);
  }

  /* wait for threads to finish */
  thread_monitor_wait(thread_monitor);
  thread_monitor_destroy(thread_monitor);

  /* deallocate data */
  for (int t = 0; t < task_count; t++) free(task_args[t]);

  return;
}
/* END opaque functions for data scaling */

void h_stretch_img_data(
    ThreadPool* thread_pool,
    uint16_t** fits_data,
    int pixel_count,
    int dim_count,
    enum PreviewMode preview_mode
    ) {
  if (preview_mode == SQUARE_ROOT) square_root_uint16_func(fits_data, pixel_count);
  if (preview_mode == AUTOSTRETCH) autostretch_uint16_func(thread_pool, fits_data, pixel_count, dim_count);
  return;
}

void h_vertical_flip_data(uint16_t** fits_data, long* dim_sizes) {
  int channel_size = dim_sizes[0] * dim_sizes[1];
  int width = dim_sizes[0];

  uint16_t temp[width];
  for (int c = 0; c < dim_sizes[2]; c++) {
    uint16_t* first_row = *fits_data + (c * channel_size);
    uint16_t* last_row = first_row + channel_size - width;
    
    while (first_row < last_row) {
      memcpy(temp, first_row, sizeof(uint16_t) * width);
      memcpy(first_row, last_row, sizeof(uint16_t) * width);
      memcpy(last_row, temp, sizeof(uint16_t) * width);
      first_row += width;
      last_row -= width;
    }
  }

  return;
}

void h_horizontal_flip_data(uint16_t** fits_data, long* dim_sizes) {
  int width = dim_sizes[0];
  int row_count = dim_sizes[1] * dim_sizes[2];

  for (int r = 0; r < row_count; r++) {
    uint16_t* start = *fits_data + (r * width); 
    uint16_t* end = *fits_data + ((r + 1) * width) - 1; 
    uint16_t temp = 0;;
    while (start < end) {
      temp = *start;
      *start = *end;
      *end = temp;
      start++;
      end--;
    }
  }

  return;
}

