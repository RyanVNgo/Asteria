#ifndef FITS_UTILS_H
#define FITS_UTILS_H

/* external libraries */
#include <fitsio.h>

/*
 * Methods for getting numerical representation of image parameters
 */
int h_fits_img_dim_count(fitsfile* fitsfile_ptr);
void h_fits_img_dim_size(fitsfile* fitsfile_ptr, long* dim_size);
LONGLONG h_fits_img_pxl_count(fitsfile* fitsfile_ptr);

/*
 * Retrieves raw data from fits file.
 * Memory allocation for *image_data is not required.
 */
void h_get_fits_img_data(fitsfile* fitsfile_ptr, float** image_data);

/********************************************************************************/
/* file routines */

void h_open_fits_file(fitsfile** fitsfile_ptr, char* fitsfile_absolute_path);
void h_save_as_fits_file(fitsfile* fitsfile_ptr, char* fitsfile_absolute_path);

void h_get_headers(fitsfile* fitsfile_ptr, char** headers_buffer);

#endif
