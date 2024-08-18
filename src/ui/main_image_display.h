#ifndef MAIN_IMAGE_DISPLAY_H
#define MAIN_IMAGE_DISPLAY_H

/* external libraries */
#include <fitsio.h>
#include <gtk/gtk.h>

/* project files */
#include "../core/threads.h"

enum PreviewMode {
  LINEAR,
  SQUARE_ROOT,
  AUTOSTRETCH
};

GtkWidget* main_image_display_get(fitsfile** current_file_ptr);

void main_image_display_load_new_image(ThreadPool* thread_pool, fitsfile** current_file_ptr);

void main_image_display_dec_image_scale(void* scale_factor_ptr);
void main_image_display_inc_image_scale(void* scale_factor_ptr);

void main_image_display_set_preview_mode(enum PreviewMode preview_mode);

#endif
