#include <gtk/gtk.h>
#include "fitsio.h"

#include "main_menu_bar.h"
#include "main_image_display.h"

static void asteria_activate(GtkApplication* app, gpointer userdata) {
  GtkWidget* window;
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Asteria");
  gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);

  GtkWidget* home_grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), home_grid);

  static fitsfile* current_file = NULL;
  GtkWidget* menu_bar = main_menu_bar_get(&current_file);
  gtk_grid_attach(GTK_GRID(home_grid), menu_bar, 0, 0, 1, 1);

  GtkWidget* image_display = main_image_display_get(&current_file);
  gtk_grid_attach(GTK_GRID(home_grid), image_display, 0, 1, 1, 1);

  gtk_widget_show_all(window);
}

int main(int argc, char* argv[]) {
  GtkApplication* app;
  int status;

  app = gtk_application_new("org.gtk.asteria", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(asteria_activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return 0;
}
