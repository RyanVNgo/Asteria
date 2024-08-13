#ifndef MAIN_IMAGE_DISPLAY_H
#define MAIN_IMAGE_DISPLAY_H

#include <gtk/gtk.h>
#include "threads.h"
#include "fitsio.h"

GtkWidget* main_image_display_get(fitsfile** current_file_ptr);

void main_image_display_load_new_image(ThreadPool* thread_pool, fitsfile** current_file_ptr);

void main_image_display_dec_image_scale(void* scale_factor_ptr);
void main_image_display_inc_image_scale(void* scale_factor_ptr);

#endif
