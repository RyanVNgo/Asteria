#ifndef IMAGE_DISPLAY_H
#define IMAGE_DISPLAY_H

/* external libraries */
#include <fitsio.h>
#include <gtk/gtk.h>

/* project files */
#include "../core/shared_data.h"

enum PreviewMode {
  LINEAR,
  SQUARE_ROOT,
  AUTOSTRETCH
};

GtkWidget* image_display_get(SharedData* shared_data);

void image_display_dec_image_scale(void* scale_factor_ptr);
void image_display_inc_image_scale(void* scale_factor_ptr);

void image_display_set_preview_mode(enum PreviewMode preview_mode);

#endif
