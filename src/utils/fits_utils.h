#ifndef FITS_UTILS_H
#define FITS_UTILS_H

/* external libraries */
#include <gtk/gtk.h>
#include <fitsio.h>

int h_fits_img_dim_count(fitsfile* fitsfile_ptr);
void h_fits_img_dim_size(fitsfile* fitsfile_ptr, long* dim_size);
LONGLONG h_fits_img_pxl_count(fitsfile* fitsfile_ptr);

void h_get_fits_img_data(fitsfile** current_file_ptr, float** image_data);

void h_fits_img_data_to_pixbuf_format(fitsfile** current_file_ptr, float** img_data, guchar** pixbuf_data, int pixel_count, int preview_mode);

/********************************************************************************/
/* file routines */

void h_open_fits_file(fitsfile** fitsfile_ptr, char* fitsfile_absolute_path);
void h_save_as_fits_file(fitsfile* fitsfile_ptr, char* fitsfile_absolute_path);

void h_get_headers(fitsfile* fitsfile_ptr, char* headers_buffer);

#endif
