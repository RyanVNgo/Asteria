/* primary header include */
#include "fits_utils.h"


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

