/* primary header include */
#include "display_img_utils.h"

void h_destroy_pixbuf_data(guchar* pixbuf_data, gpointer data) {
  free(pixbuf_data);
  pixbuf_data = NULL;
  return;
}

void h_display_img_adj_scale(GtkWidget* display_img, GdkPixbuf* unscaled_pixbuf, float scale) {
  int scaled_width = gdk_pixbuf_get_width(unscaled_pixbuf) * scale;
  int scaled_height = gdk_pixbuf_get_height(unscaled_pixbuf) * scale;

  GdkPixbuf* scaled_pixbuf;
  scaled_pixbuf = gdk_pixbuf_scale_simple(unscaled_pixbuf, scaled_width, scaled_height, GDK_INTERP_TILES);

  gtk_image_set_from_pixbuf(GTK_IMAGE(display_img), scaled_pixbuf);

  g_object_unref(scaled_pixbuf);
  return;
}

void h_display_img_flip(GtkWidget* display_img, GdkPixbuf** unscaled_pixbuf, gboolean horizontal) {
  /* flip unscaled_pixbuf of shared data */
  GdkPixbuf* unscaled_pixbuf_flipped = gdk_pixbuf_flip(*unscaled_pixbuf, horizontal);
  g_object_unref(*unscaled_pixbuf);
  *unscaled_pixbuf = unscaled_pixbuf_flipped;

  /* flip pixbuf of the displayed image */
  GdkPixbuf* curr_pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(display_img));
  GdkPixbuf* flipped_pixbuf = gdk_pixbuf_flip(curr_pixbuf, horizontal);
  gtk_image_set_from_pixbuf(GTK_IMAGE(display_img), flipped_pixbuf);

  g_object_unref(flipped_pixbuf);
  return;
}
