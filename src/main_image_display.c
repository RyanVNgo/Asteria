#include "main_image_display.h"

#include "main_options_bar.h"

#include "app_cfitsio_helper.h"

#include "threads.h"
#include <pthread.h>

static GtkWidget* image_scrolled_window;
static GtkWidget* image;
static GdkPixbuf* base_pixbuf;
static guchar* pixbuf_data;

GtkWidget* main_image_display_get(fitsfile** current_file_ptr) {
  GtkWidget* image_grid = gtk_grid_new();

  image_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_widget_set_hexpand(image_scrolled_window, TRUE);
  gtk_widget_set_vexpand(image_scrolled_window, TRUE);

  image = gtk_image_new();

  gtk_container_add(GTK_CONTAINER(image_scrolled_window), image);
  gtk_grid_attach(GTK_GRID(image_grid), image_scrolled_window, 0, 1, 1, 1);
  
  return image_grid;
}

void main_image_display_load_new_image(ThreadPool* thread_pool, fitsfile** current_file_ptr) {
  int status = 0;
  int width = 0, height = 0, channels = 0;
  hcfitsio_get_image_dimensions(current_file_ptr, &width, &height, &channels, &status);
  if (status) return;
  
  float* img_data;
  int pixel_count = width * height * channels;

  hcfitsio_get_image_data(current_file_ptr, &img_data);
  
  if (pixbuf_data) g_free(pixbuf_data);
  pixbuf_data = (guchar*)malloc(sizeof(guchar) * pixel_count);
  
  hcfitsio_img_data_to_pixbuf_format(thread_pool, current_file_ptr, &img_data, &pixbuf_data, pixel_count);
  
  base_pixbuf = gdk_pixbuf_new_from_data(
      pixbuf_data,
      GDK_COLORSPACE_RGB,
      FALSE,
      8,
      width,
      height,
      width * channels,
      NULL,
      NULL
  );

  gtk_image_set_from_pixbuf(GTK_IMAGE(image), base_pixbuf);
  main_options_bar_reset_scale_factor();
  
  g_free(img_data);
  return;
}

void main_image_display_dec_image_scale(void* scale_factor_ptr) {
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

void main_image_display_inc_image_scale(void* scale_factor_ptr) {
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
