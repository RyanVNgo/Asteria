#ifndef MAIN_IMAGE_DISPLAY_H
#define MAIN_IMAGE_DISPLAY_H

#include <gtk/gtk.h>
#include "fitsio.h"

static GtkWidget* image_box;
static GtkWidget* image;

GtkWidget* main_image_display_get(fitsfile** current_file_ptr);
void main_image_display_load_new_image(fitsfile** current_file_ptr);

#endif
