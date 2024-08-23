#ifndef DISPLAY_IMG_UTILS_H
#define DISPLAY_IMG_UTILS_H

/* external libraries */
#include <gtk/gtk.h>

void h_destroy_pixbuf_data(guchar* pixbuf_data, gpointer data);
void h_display_img_adj_scale(GtkWidget* display_img, GdkPixbuf* unscaled_pixbuf, float scale);

#endif
