/* primary header include */
#include "image_display.h"

GtkWidget* image_display_get(SharedData* shared_data) {
  GtkWidget* image_grid = gtk_grid_new();

  GtkWidget* image_scrolled_window;
  image_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_widget_set_hexpand(image_scrolled_window, TRUE);
  gtk_widget_set_vexpand(image_scrolled_window, TRUE);

  shared_data->display_image = gtk_image_new();

  gtk_container_add(GTK_CONTAINER(image_scrolled_window), shared_data->display_image);
  gtk_grid_attach(GTK_GRID(image_grid), image_scrolled_window, 0, 0, 1, 1);
  
  return image_grid;
}

