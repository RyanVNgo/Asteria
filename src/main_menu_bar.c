#include "main_menu_bar.h"
#include "gtk/gtk.h"
#include "menu_factory.h"
#include "file_handler.h"

gboolean on_window_delete(GtkWidget* widget, gpointer data) {
  gtk_widget_hide(widget);
  return TRUE;
}
static void on_window_activate(GtkWidget* menu_item, gpointer window) {
  if (gtk_widget_get_visible(window)) {
    gtk_widget_hide(window);
    return;
  }
  gtk_widget_show_all(window);
  return;
}

/**************************************************/
/* File */

void open_item_activate(GtkWidget* menu_item, gpointer data) {
  GFile* file = fh_get_file(NULL, NULL);
  if (!file) return;
  char* filename = g_file_get_path(file);
  g_print("Filename: %s\n", filename);
  g_free(file);
  g_free(filename);
}

static GtkWidget* menu_bar_file_item() {
  GtkWidget* file_item = mf_menu_item_with_submenu_init("File");

  GtkWidget* open_item = gtk_menu_item_new_with_label("Open");
  g_signal_connect(open_item, "activate", G_CALLBACK(open_item_activate), NULL);

  GtkWidget* save_item = gtk_menu_item_new_with_label("Save");
  GtkWidget* saveas_item = gtk_menu_item_new_with_label("Save As");

  mf_menu_item_add_menu_item(file_item, open_item);
  mf_menu_item_add_menu_item(file_item, save_item);
  mf_menu_item_add_menu_item(file_item, saveas_item);

  return file_item;
}

/**************************************************/
/* Help */

static GtkWidget* about_window_init() {
  GtkWidget* about_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(about_window), "About");
  gtk_window_set_default_size(GTK_WINDOW(about_window), 480, 320);
  gtk_window_set_resizable(GTK_WINDOW(about_window), FALSE);

  GtkWidget* text = gtk_label_new("Developer - Ryan V Ngo");
  gtk_label_set_angle(GTK_LABEL(text), 23.8);
  gtk_container_add(GTK_CONTAINER(about_window), text);

  return about_window;
}

static GtkWidget* menu_bar_help_item() {
  GtkWidget* help_item = mf_menu_item_with_submenu_init("Help");

  GtkWidget* about_window = about_window_init();
  GtkWidget* about_item = gtk_menu_item_new_with_label("About");
  g_signal_connect(about_item, "activate", G_CALLBACK(on_window_activate), about_window);
  g_signal_connect(about_window, "delete-event", G_CALLBACK(on_window_delete), NULL);

  GtkWidget* docs_item = gtk_menu_item_new_with_label("Docs");

  mf_menu_item_add_menu_item(help_item, about_item);
  mf_menu_item_add_menu_item(help_item, docs_item);

  return help_item;
}

/**************************************************/
/* Menu Bar */

GtkWidget* main_menu_bar_get() {
  GtkWidget* menu_bar = gtk_menu_bar_new();
  
  GtkWidget* file_item = menu_bar_file_item();
  GtkWidget* help_item = menu_bar_help_item();

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_item);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help_item);

  return menu_bar;
}
