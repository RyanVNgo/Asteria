#ifndef FITS_UTILS_H
#define FITS_UTILS_H

/* external libraries */
#include <gtk/gtk.h>
#include <fitsio.h>

/* project files */
#include "../core/shared_data.h"
#include "../core/threads.h"

int h_fits_img_dim_count(fitsfile* fitsfile_ptr);
void h_fits_img_dim_size(fitsfile* fitsfile_ptr, long* dim_size);
LONGLONG h_fits_img_pxl_count(fitsfile* fitsfile_ptr);

void h_get_fits_img_data(fitsfile* fitsfile_ptr, float** image_data);
void h_float_to_uint16_array(float** float_data, uint16_t** uint16_buff, int pixel_count);

void h_scale_fits_data(ThreadPool* thread_pool, uint16_t** fits_data, int pixel_count, int dim_count, enum PreviewMode preview_mode);

void h_uint16_to_pixbuf_format(uint16_t** uint16_data, guchar** pixbuf_buff, int pixel_count);
void h_fits_img_data_to_pixbuf_format(fitsfile* fitsfile_ptr, float** img_data, guchar** pixbuf_data, int pixel_count);

/********************************************************************************/
/* file routines */

void h_open_fits_file(fitsfile** fitsfile_ptr, char* fitsfile_absolute_path);
void h_save_as_fits_file(fitsfile* fitsfile_ptr, char* fitsfile_absolute_path);

void h_get_headers(fitsfile* fitsfile_ptr, char** headers_buffer);

#endif
