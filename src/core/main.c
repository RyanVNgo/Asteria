#include <gtk/gtk.h>
#include "fitsio.h"

#include "threads.h"
#include "shared_data.h"
#include "../ui/main_image_display.h"
#include "../ui/main_menu_bar.h"
#include "../ui/main_options_bar.h"

static void asteria_activate(GtkApplication* app, gpointer thread_pool_in) {
  ThreadPool* thread_pool = (ThreadPool*)thread_pool_in;
  fitsfile* current_file = NULL;

  SharedData* shared_data = malloc(sizeof(SharedData));
  shared_data->thread_pool = thread_pool;
  shared_data->current_file = current_file;

  GtkWidget* window;
  gint default_width = 1200;
  gint default_height = 900;
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Asteria");
  gtk_window_set_default_size(GTK_WINDOW(window), default_width, default_height);
  
  GtkWidget* home_grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), home_grid);

  GtkWidget* menu_bar = main_menu_bar_get(shared_data);
  gtk_grid_attach(GTK_GRID(home_grid), menu_bar, 0, 0, 1, 1);

  GtkWidget* options_bar = main_options_bar_get(shared_data);
  gtk_grid_attach(GTK_GRID(home_grid), options_bar, 0, 1, 1, 1);

  GtkWidget* image_display = main_image_display_get(&current_file);
  gtk_grid_attach(GTK_GRID(home_grid), image_display, 0, 2, 1, 1);

  gtk_widget_show_all(window);
}

int main(int argc, char* argv[]) {
  GtkApplication* app;
  int status;
  
  ThreadPool* thread_pool;
  const int max_threads = 4; /* This must always be greater than 0 */
  thread_pool = thread_pool_init(max_threads);
  
  app = gtk_application_new("org.gtk.asteria", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(asteria_activate), thread_pool);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  thread_pool_shutdown(thread_pool);

  return 0;
}
