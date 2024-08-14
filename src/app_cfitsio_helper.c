#include "app_cfitsio_helper.h"

#include "fitsio.h"
#include "main_image_display.h"
#include "threads.h"

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


struct idtpbf_args {
  ThreadMonitor* thread_monitor;
  float** img_data;
  guchar** pixbuf_data;
  int channel;
  int channel_size;
  enum PreviewMode preview_mode;
} idtpbf_args;

float linear_scale_func(float data_val) {
  return data_val;
}
float logarithm_scale_func(float data_val) {
  return data_val;
}
float square_root_scale_func(float data_val) {
  return sqrt(data_val);
}
float autostretch_root_scale_func(float data_val) {
  return data_val;
}

void img_data_to_pixbuf_format_thr(void* args) {
  struct idtpbf_args* parsed_args = (struct idtpbf_args*)args;
  ThreadMonitor* thread_monitor = parsed_args->thread_monitor;
  float** img_data = parsed_args->img_data;
  guchar** pixbuf_data = parsed_args->pixbuf_data;
  int channel = parsed_args->channel;
  int channel_size = parsed_args->channel_size;
  enum PreviewMode preview_mode = parsed_args->preview_mode;

  const guchar guchar_max = 255;

  float (*scaling_func)(float);
  switch (preview_mode) {
    case LINEAR:
      scaling_func = linear_scale_func;
      break;
    case LOGARITHM:
      scaling_func = logarithm_scale_func;
      break;
    case SQUARE_ROOT:
      scaling_func = square_root_scale_func;
      break;
    case AUTOSTRETCH:
      scaling_func = autostretch_root_scale_func;
      break;
    default:
      scaling_func = linear_scale_func;
  }

  float data_val = 0;
  int pixbuf_val= 0;
  int i = channel;
  for (int p = channel * channel_size; p < (channel+1) * channel_size; p++) {
    data_val = scaling_func((*img_data)[p]);
    pixbuf_val = data_val * guchar_max;
    if (pixbuf_val > guchar_max) pixbuf_val = guchar_max;
    (*pixbuf_data)[i] = pixbuf_val;
    i += 3;
  }

  thread_monitor_signal(thread_monitor);
  return;
}

void hcfitsio_img_data_to_pixbuf_format(ThreadPool* thread_pool, fitsfile** current_file_ptr, float** img_data, guchar** pixbuf_data, int pixel_count, int preview_mode) {
  const guchar guchar_max = 255;

  int maxdim = 0;
  int status = 0;
  fits_get_img_dim(*current_file_ptr, &maxdim, &status);

  if (status) {
    fits_report_error(stderr, status);
    return;
  }

  long naxes[maxdim];
  fits_get_img_size(*current_file_ptr, maxdim, naxes, &status);
  if (status) {
    fits_report_error(stderr, status);
    return;
  }

  int channel_size = naxes[0] * naxes[1];
  float data_val = 0;
  int norm_val = 0;

  int task_count = 3;
  ThreadMonitor* thread_monitor = thread_monitor_init(task_count);
  struct idtpbf_args* task_args[3];

  
  for (int i = 0; i < naxes[2]; i++) {
    task_args[i] = malloc(sizeof(struct idtpbf_args));
    task_args[i]->thread_monitor = thread_monitor;
    task_args[i]->img_data = img_data;
    task_args[i]->pixbuf_data = pixbuf_data;
    task_args[i]->channel = i;
    task_args[i]->channel_size = channel_size;
    task_args[i]->preview_mode = preview_mode;
    submit_task(thread_pool, img_data_to_pixbuf_format_thr, task_args[i]);
  }

  thread_monitor_wait(thread_monitor);
  thread_monitor_destroy(thread_monitor);
  return;
}


void hcfitsio_save_file(fitsfile** current_file_ptr, const char* absolute_path, int* status) {
  fitsfile* new_fptr;

  fits_create_file(&new_fptr, absolute_path, status);
  if (*status == FILE_NOT_CREATED) {
    char* modified_absolute_path = g_strdup_printf("!%s", absolute_path);
    fits_create_file(&new_fptr, modified_absolute_path, status);
  }
  fits_copy_file(*current_file_ptr, new_fptr, 1, 1, 1, status);

  fits_close_file(new_fptr, status);
  return;
}

void hcfitsio_get_image_dimensions(fitsfile** current_file_ptr, int* width, int* height, int* channels, int* status) {
  if (!*current_file_ptr) return;
  
  int maxdim = 0;
  fits_get_img_dim(*current_file_ptr, &maxdim, status);
  if (*status) {
    fits_report_error(stderr, *status);
    return;
  }

  long naxes[maxdim];
  fits_get_img_size(*current_file_ptr, maxdim, naxes, status);
  if (*status) {
    fits_report_error(stderr, *status);
    return;
  }

  *width = (int)naxes[0];
  *height = (int)naxes[1];
  *channels = (int)naxes[2];

  return;
}

void hcfitsio_get_image_data(fitsfile** current_file_ptr, float** image_data) {
  int status = 0;
  int width = 0, height = 0, channels = 0;
  hcfitsio_get_image_dimensions(current_file_ptr, &width, &height, &channels, &status);

  int chdunum = 0;
  fits_get_hdu_num(*current_file_ptr, &chdunum);

  fits_movabs_hdu(*current_file_ptr, 1, NULL, &status);

  long naxis_start[3] = {1, 1, 1};
  LONGLONG n_elements = width * height * channels;

  int bitpix;
  fits_get_img_type(*current_file_ptr, &bitpix, &status);

  int data_type = bitpix_to_datatype(bitpix);
  *image_data = (float*)malloc(n_elements * sizeof(float));
  fits_read_pix(*current_file_ptr, data_type, naxis_start, n_elements, NULL, *image_data, NULL, &status);

  return;
}

