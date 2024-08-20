/* primary header include */
#include "display_img_utils.h"

void destroy_pixbuf_data(guchar* pixbuf_data, gpointer data) {
  free(pixbuf_data);
  return;
}

void display_img_adj_scale(GtkWidget* display_img, GdkPixbuf* unscaled_pixbuf, float scale) {
  int scaled_width = gdk_pixbuf_get_width(unscaled_pixbuf) * scale;
  int scaled_height = gdk_pixbuf_get_height(unscaled_pixbuf) * scale;

  GdkPixbuf* scaled_pixbuf;
  scaled_pixbuf = gdk_pixbuf_scale_simple(unscaled_pixbuf, scaled_width, scaled_height, GDK_INTERP_TILES);

  gtk_image_set_from_pixbuf(GTK_IMAGE(display_img), scaled_pixbuf);

  g_object_unref(scaled_pixbuf);
  return;
}

