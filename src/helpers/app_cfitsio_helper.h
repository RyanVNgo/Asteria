#ifndef APP_CFITSIO_HELPER_H
#define APP_CFITSIO_HELPER_H

/* external libraries */
#include <gtk/gtk.h>
#include "fitsio.h"

/* project files */
#include "../core/threads.h"

void hcfitsio_open_file(fitsfile** fitsfile_ptr, char* fitsfile_absolute_path);
void hcfitsio_save_as_file(fitsfile* fitsfile_ptr, char* fitsfile_absolute_path);

void hcfitsio_get_image_dimensions(fitsfile** current_file_ptr, int* width, int* height, int* channels, int* status);

void hcfitsio_get_image_data(fitsfile** current_file_ptr, float** image_data);

void hcfitsio_img_data_to_pixbuf_format(ThreadPool* thread_pool, fitsfile** current_file_ptr, float** img_data, guchar** pixbuf_data, int pixel_count, int preview_mode);

#endif
