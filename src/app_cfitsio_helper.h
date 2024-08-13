#ifndef APP_CFITSIO_HELPER_H
#define APP_CFITSIO_HELPER_H

#include <pthread.h>
#include <gtk/gtk.h>
#include "fitsio.h"
#include "threads.h"

void hcfitsio_save_file(fitsfile** current_file_ptr, const char* root_filename, int* status);
void hcfitsio_get_image_dimensions(fitsfile** current_file_ptr, int* width, int* height, int* channels, int* status);

void hcfitsio_get_image_data(fitsfile** current_file_ptr, float** image_data);

void hcfitsio_img_data_to_pixbuf_format(ThreadPool* thread_pool, fitsfile** current_file_ptr, float** img_data, guchar** pixbuf_data, int pixel_count);

#endif
