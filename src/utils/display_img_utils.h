#ifndef DISPLAY_IMG_UTILS_H
#define DISPLAY_IMG_UTILS_H

/* external libraries */
#include "gdk-pixbuf/gdk-pixbuf.h"
#include <gtk/gtk.h>

void display_img_adj_scale(GtkWidget* display_img, GdkPixbuf* unscaled_pixbuf, float scale);

#endif
