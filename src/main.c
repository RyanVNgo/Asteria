#include <gtk/gtk.h>

static void open_separate_window(GtkWidget* widget, gpointer separate_window) {
  if (separate_window == NULL)
    return;
  if (gtk_widget_get_visible(separate_window)) {
    gtk_widget_hide(separate_window);
  } else {
    gtk_widget_show_all(separate_window);
  }
  return;
}

gboolean on_widget_deleted(GtkWidget* widget, gpointer data) {
  gtk_widget_hide(widget);
  return TRUE;
}

static void asteria_activate(GtkApplication* app, gpointer userdata) {
  GtkWidget* window;
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Asteria");
  gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);

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
