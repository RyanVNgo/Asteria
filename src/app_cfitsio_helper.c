#include "app_cfitsio_helper.h"
#include "fitsio.h"
#include "glib.h"
#include "longnam.h"

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

void hcfitsio_img_data_to_pixbuf_format(fitsfile** current_file_ptr, float** img_data, guchar** pixbuf_data, int pixel_count) {
  *pixbuf_data = (guchar*)malloc(sizeof(guchar) * pixel_count);

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
  
  float avg_val = 0;
  for (int i = 0; i < pixel_count; i++) {
    avg_val += (*img_data)[i];
  }
  avg_val /= pixel_count;

  float scaler = 0.1 / avg_val;

  for (int c = 0; c < naxes[2]; c++) {
    int i = c;
    for (int p = c * channel_size; p < (c+1) * channel_size; p++) {
      data_val = (*img_data)[p] * scaler;
      norm_val = data_val * 255;
      if (norm_val > 255) norm_val = 255;
      (*pixbuf_data)[i] = norm_val;
      i += 3;
    }
  }

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

