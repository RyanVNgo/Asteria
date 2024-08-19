/* primary header include */
#include "image_display.h"

static GtkWidget* image_scrolled_window;
static GtkWidget* image;
static GdkPixbuf* base_pixbuf;
static guchar* pixbuf_data;
static enum PreviewMode preview_mode = LINEAR;

GtkWidget* image_display_get(SharedData* shared_data) {
  GtkWidget* image_grid = gtk_grid_new();

  image_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_widget_set_hexpand(image_scrolled_window, TRUE);
  gtk_widget_set_vexpand(image_scrolled_window, TRUE);

  shared_data->display_image = gtk_image_new();

  gtk_container_add(GTK_CONTAINER(image_scrolled_window), shared_data->display_image);
  gtk_grid_attach(GTK_GRID(image_grid), image_scrolled_window, 0, 0, 1, 1);
  
  return image_grid;
}

/*
void image_display_dec_image_scale(void* scale_factor_ptr) {
  float scale_factor = *(float*)(scale_factor_ptr);
  if (!base_pixbuf) {
    return;
  }

  if (scale_factor == 0.25) return;
  scale_factor /= 2.0;

  int scaled_width = gdk_pixbuf_get_width(base_pixbuf) * scale_factor;
  int scaled_height = gdk_pixbuf_get_height(base_pixbuf) * scale_factor;
  GdkPixbuf* scaled_pixbuf = gdk_pixbuf_scale_simple(base_pixbuf, scaled_width, scaled_height, GDK_INTERP_TILES);

  gtk_image_set_from_pixbuf(GTK_IMAGE(image), scaled_pixbuf);

  *(float*)(scale_factor_ptr) = scale_factor;
  g_object_unref(scaled_pixbuf);
  return;
}

void image_display_inc_image_scale(void* scale_factor_ptr) {
  float scale_factor = *(float*)(scale_factor_ptr);
  if (!base_pixbuf) return;

  if (scale_factor == 4.0) return;
  scale_factor *= 2.0;

  int scaled_width = gdk_pixbuf_get_width(base_pixbuf) * scale_factor;
  int scaled_height = gdk_pixbuf_get_height(base_pixbuf) * scale_factor;
  GdkPixbuf* scaled_pixbuf = gdk_pixbuf_scale_simple(base_pixbuf, scaled_width, scaled_height, GDK_INTERP_TILES);

  gtk_image_set_from_pixbuf(GTK_IMAGE(image), scaled_pixbuf);

  *(float*)(scale_factor_ptr) = scale_factor;
  g_object_unref(scaled_pixbuf);
  return;
}
*/

void image_display_set_preview_mode(enum PreviewMode preview_mode_in) {
  if (preview_mode_in == LINEAR ||
      preview_mode_in == SQUARE_ROOT ||
      preview_mode_in == AUTOSTRETCH ) {
    preview_mode = preview_mode_in;
  }
  return;
}
