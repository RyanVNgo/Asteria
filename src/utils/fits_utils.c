/* primary header include */
#include "fits_utils.h"

/* external libraries */
#include <math.h>

int bitpix_to_datatype(int bitpix) {
  switch(bitpix) {
    case BYTE_IMG:
      return TBYTE;
    case SHORT_IMG:
      return TSHORT;
    case LONG_IMG:
      return TLONG;
    case LONGLONG_IMG:
      return TLONGLONG;
    case FLOAT_IMG:
      return TFLOAT;
    case DOUBLE_IMG:
      return TDOUBLE;
    default:
      return -1;
  }
}

int h_fits_img_dim_count(fitsfile* fitsfile_ptr) {
  int status = 0;
  int dim_count = 0;
  fits_get_img_dim(fitsfile_ptr, &dim_count, &status);
  return dim_count;
}

void h_fits_img_dim_size(fitsfile* fitsfile_ptr, long* dim_size) {
  int status = 0;
  fits_get_img_size(fitsfile_ptr, h_fits_img_dim_count(fitsfile_ptr), dim_size, &status);
  return;
}

LONGLONG h_fits_img_pxl_count(fitsfile* fitsfile_ptr) {
  LONGLONG pxl_count = 1;
  int dim_count = h_fits_img_dim_count(fitsfile_ptr);
  long dim_size[dim_count];
  h_fits_img_dim_size(fitsfile_ptr, dim_size);
  for (int d = 0; d < dim_count; d++) {
    pxl_count *= dim_size[d];
  }
  return pxl_count;
}

void h_get_fits_img_data(fitsfile* fitsfile_ptr, float** image_data) {
  int status = 0;
  
  /* get data type of fits file */
  int bitpix;
  fits_get_img_type(fitsfile_ptr, &bitpix, &status);
  int data_type = bitpix_to_datatype(bitpix);

  /* get total pixel count */
  int pixel_count = h_fits_img_pxl_count(fitsfile_ptr);

  /* extract raw data from fits file */
  long naxis_start[3] = {1, 1, 1};
  *image_data = (float*)malloc(pixel_count * sizeof(float));
  fits_read_pix(fitsfile_ptr, data_type, naxis_start, pixel_count, NULL, *image_data, NULL, &status);

  return;
}

void h_float_to_uint16_array(float** float_data, uint16_t** uint16_buff, int pixel_count) {
  for (int i = 0; i < pixel_count; i++) {
    *(*uint16_buff + i) = (uint16_t)(*(*float_data + i) * UINT16_MAX);
  }
  return;
}

/* START opaque functions for data scaling */
void square_root_uint16_func(uint16_t** img_data, int pixel_count) {
  int i = 0;
  for (; i < pixel_count; i++) {
    *(*img_data + i) = sqrt(*(*img_data + i) /  (float)UINT16_MAX) * UINT16_MAX;
  }
  return;
}

void autostretch_uint16_thread_func(void* args_in) {
  struct func_args {
    ThreadMonitor* thread_monitor;
    uint16_t* img_data;
    int pixel_count;
    float pow_val;
  };
  struct func_args* args = (struct func_args*)args_in;

  for (int i = 0; i < args->pixel_count; i++) {
    *(args->img_data + i) = pow(*(args->img_data + i) / (float)UINT16_MAX, args->pow_val) * UINT16_MAX;
  }

  thread_monitor_signal(args->thread_monitor);
  return;
}

void autostretch_uint16_func(ThreadPool* thread_pool, uint16_t** fits_data, int pixel_count, int dim_count) {
  int channel_size = pixel_count / dim_count;
  unsigned long long dim_avg[dim_count];
  float pow_val[dim_count];
  float t_val = 0.2;

  /* find pixl avg per channel and pow val */
  int i = 0;
  int c = 0;
  for (c = 0; c < dim_count; c++) {
    dim_avg[c] = 0;
    i = 0;
    for(; i < channel_size; i++) {
      dim_avg[c] += *(*fits_data+ i + (channel_size * c));
    }
    dim_avg[c] /= channel_size;
    pow_val[c] = log(t_val) / log(dim_avg[c] / (float)UINT16_MAX);
  }

  /* multi-threaded implementation START */

  /* define arg struct */
  struct func_args {
    ThreadMonitor* thread_monitor;
    uint16_t* img_data;
    int pixel_count;
    float pow_val;
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
    task_args[t]->pow_val = pow_val[t];
    submit_task(thread_pool, autostretch_uint16_thread_func, task_args[t]);
  }

  /* wait for threads to finish */
  thread_monitor_wait(thread_monitor);
  thread_monitor_destroy(thread_monitor);

  /* deallocate data */
  for (int t = 0; t < task_count; t++) free(task_args[t]);
  
  /* multi-threaded implementation END */

  return;
}

void h_scale_fits_data(
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

void h_uint16_to_pixbuf_format(uint16_t** uint16_data, guchar** pixbuf_buff, int pixel_count) {
  int channel_size = pixel_count / 3;
  for (int c = 0; c < 3; c++) {
    int i = c;
    for (int p = c * channel_size; p < (c + 1) * channel_size; p++) {
      (*pixbuf_buff)[i] = (*uint16_data)[p]  / UCHAR_MAX;
      if ((*pixbuf_buff)[i] > UCHAR_MAX) (*pixbuf_buff)[i] = UCHAR_MAX;
      i += 3;
    }
  }
  return;
}

void h_fits_img_data_to_pixbuf_format(fitsfile* fitsfile_ptr, float** img_data, guchar** pixbuf_data, int pixel_count) {
  int status = 0;

  int maxdim = 0;
  fits_get_img_dim(fitsfile_ptr, &maxdim, &status);

  long naxes[maxdim];
  fits_get_img_size(fitsfile_ptr, maxdim, naxes, &status);
  int channel_size = naxes[0] * naxes[1];
  
  for (int c = 0; c < naxes[2]; c++) {
    int i = c;
    for (int p = c* channel_size; p < (c+1) * channel_size; p++) {
      (*pixbuf_data)[i] = (*img_data)[p] * UCHAR_MAX; 
      if ((*pixbuf_data)[i] > UCHAR_MAX) (*pixbuf_data)[i] = UCHAR_MAX;
      i += 3;
    }
  }

  return;
}

/********************************************************************************/
/* file routines */

void h_open_fits_file(fitsfile** fitsfile_ptr, char* fitsfile_absolute_path) {
  int status = 0;
  if (*fitsfile_ptr) fits_close_file(*fitsfile_ptr, &status);
  
  fits_open_file(fitsfile_ptr, fitsfile_absolute_path, READONLY, &status);

  if (status) {
    fits_report_error(stderr, status);
    fitsfile_ptr = NULL;
  }

  return;
}

void h_save_as_fits_file(fitsfile* fitsfile_ptr, char* fitsfile_absolute_path) {
  int status = 0;
  fitsfile* new_fptr;

  fits_create_file(&new_fptr, fitsfile_absolute_path, &status);
  
  /* case for when file path already exists (overwriting an existing file) */
  if (status == FILE_NOT_CREATED) {
    char* modified_absolute_path = g_strdup_printf("!%s", fitsfile_absolute_path);
    fits_create_file(&new_fptr, modified_absolute_path, &status);
  }
  fits_copy_file(fitsfile_ptr, new_fptr, 1, 1, 1, &status);
  
  fits_close_file(new_fptr, &status);
  return;
}

/* 
 * allocs and fills headers_buffer with headers as single string 
 * likey change to more dynamic structure like a multi-dim array
 */
void h_get_headers(fitsfile* fitsfile_ptr, char** headers_buffer) {
  int status = 0;

  int card_count = 0;
  fits_get_hdrspace(fitsfile_ptr, &card_count, NULL, &status);

  char card[FLEN_CARD];
  int buffer_idx = 0;
  int card_length = 0;
  for (int i = 1; i <= card_count; i++) {
    fits_read_record(fitsfile_ptr, i, card, &status);

    card_length = 0;
    while (card[card_length] != '\0') card_length++;

    /* probably not best to do constant reallocs but it works for now */
    /* maybe change in the future alongside structure change */
    *headers_buffer = realloc(*headers_buffer, buffer_idx + card_length + 1);
    strcpy(*headers_buffer + buffer_idx, card);
    buffer_idx += card_length + 1;
    (*headers_buffer)[buffer_idx - 1] = '\n';
  }
  (*headers_buffer)[buffer_idx - 1] = '\0';

  return;
}

