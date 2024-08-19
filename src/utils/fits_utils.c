/* primary header include */
#include "fits_utils.h"

/* external libraries */
#include <math.h>

/* project files */
#include "../core/threads.h"
#include "../ui/image_display.h"

#define GUCHAR_MAX 255

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

void h_fits_img_data_to_pixbuf_format(fitsfile** current_file_ptr, float** img_data, guchar** pixbuf_data, int pixel_count, int preview_mode) {
  int status = 0;

  int maxdim = 0;
  fits_get_img_dim(*current_file_ptr, &maxdim, &status);

  long naxes[maxdim];
  fits_get_img_size(*current_file_ptr, maxdim, naxes, &status);

  int channel_size = naxes[0] * naxes[1];
  float data_val = 0;
  int norm_val = 0;
  
  float (*scaling_func)(float);
  switch (preview_mode) {
    case LINEAR:
      scaling_func = linear_scale_func;
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

  int pixbuf_val= 0;

  for (int c = 0; c < naxes[2]; c++) {
    int i = c;
    for (int p = c* channel_size; p < (c+1) * channel_size; p++) {
      data_val = scaling_func((*img_data)[p]);
      pixbuf_val = data_val * GUCHAR_MAX;
      if (pixbuf_val > GUCHAR_MAX) pixbuf_val = GUCHAR_MAX;
      (*pixbuf_data)[i] = pixbuf_val;
      i += 3;
    }
  }

  return;
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

void h_get_fits_img_data(fitsfile** current_file_ptr, float** image_data) {
  int status = 0;
  
  /* get data type of fits file */
  int bitpix;
  fits_get_img_type(*current_file_ptr, &bitpix, &status);
  int data_type = bitpix_to_datatype(bitpix);

  /* get total pixel count */
  int pixel_count = h_fits_img_pxl_count(*current_file_ptr);

  /* extract raw data from fits file */
  long naxis_start[3] = {1, 1, 1};
  *image_data = (float*)malloc(pixel_count * sizeof(float));
  fits_read_pix(*current_file_ptr, data_type, naxis_start, pixel_count, NULL, *image_data, NULL, &status);

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

