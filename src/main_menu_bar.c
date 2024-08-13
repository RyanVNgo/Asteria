#include "fitsio.h"

#include "main_menu_bar.h"
#include "main_image_display.h"

#include "app_gtk_helper.h"
#include "app_glib_helper.h"
#include "app_cfitsio_helper.h"
#include "shared_data.h"

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

void open_item_activate(GtkWidget* menu_item, SharedData* shared_data) {
  char* file_absolute_path = hglib_get_absolute_path();
  if (!file_absolute_path) return;

  int status = 0;
  fitsfile* temp_file_ptr;

  fits_open_file(&temp_file_ptr, file_absolute_path, READONLY, &status);
  if (status) {
    fits_report_error(stderr, status);
    return;
  }

  if (shared_data->current_file) {
    fits_close_file(shared_data->current_file, &status);
    if (status) {
      fits_report_error(stderr, status);
      return;
    }
  }

  shared_data->current_file = temp_file_ptr;

  if (shared_data->current_file && !status) {
    g_print("File -> Open: %s\n", file_absolute_path);
    main_image_display_load_new_image(shared_data->thread_pool, &shared_data->current_file);
  } else {
    g_print("File -> Open: ERROR\n");
    fits_report_error(stderr, status);
  }

  return;
}

void saveas_item_activate(GtkWidget* menu_item, SharedData* shared_data) {
  if (shared_data->current_file == NULL) return;

  int status = 0;
  char* saveas_absolute_path = hglib_saveas_absolute_path();

  if (!saveas_absolute_path) return;
  hcfitsio_save_file(&shared_data->current_file, saveas_absolute_path, &status);

  if (!status) {
    g_print("File -> Save As: %s\n", saveas_absolute_path);
  } else {
    fits_report_error(stderr, status);
  }

  return;
}

static GtkWidget* menu_bar_file_item(SharedData* shared_data) {
  GtkWidget* file_item = hgtk_menu_item_with_submenu_init("File");

  GtkWidget* open_item = gtk_menu_item_new_with_label("Open");
  g_signal_connect(open_item, "activate", G_CALLBACK(open_item_activate), shared_data);

  GtkWidget* save_item = gtk_menu_item_new_with_label("Save");

  GtkWidget* saveas_item = gtk_menu_item_new_with_label("Save As");
  g_signal_connect(saveas_item, "activate", G_CALLBACK(saveas_item_activate), shared_data);

  hgtk_menu_item_add_menu_item(file_item, open_item);
  hgtk_menu_item_add_menu_item(file_item, save_item);
  hgtk_menu_item_add_menu_item(file_item, saveas_item);

  return file_item;
}

/**************************************************/
/* Info */

void headers_item_activate(GtkWidget* menu_item, fitsfile** current_file_ptr) {
  if (!*current_file_ptr) return;
  int status = 0;
  int card_count;

  fits_get_hdrspace(*current_file_ptr, &card_count, NULL, &status);
  if (status) return fits_report_error(stderr, status);

  char card[FLEN_CARD];
  int i = 1;
  for(; i < card_count; i++) {
    fits_read_record(*current_file_ptr, i, card, &status);
    g_print("%s\n", card);
  }

  return;
}

static GtkWidget* menu_bar_info_item(fitsfile** current_file_ptr) {
  GtkWidget* info_item = hgtk_menu_item_with_submenu_init("Info");

  GtkWidget* headers_item = gtk_menu_item_new_with_label("Headers");
  g_signal_connect(headers_item, "activate", G_CALLBACK(headers_item_activate), current_file_ptr);

  hgtk_menu_item_add_menu_item(info_item, headers_item);

  return info_item;
}

/**************************************************/
/* Help */

static GtkWidget* about_window_init() {
  GtkWidget* about_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(about_window), "About");
  gtk_window_set_default_size(GTK_WINDOW(about_window), 480, 320);
  gtk_window_set_resizable(GTK_WINDOW(about_window), FALSE);

  GtkWidget* grid = gtk_grid_new();
  gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
  gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

  GtkWidget* about_text = gtk_label_new("Developer - Ryan V Ngo\nVersion - 0.0.1\nDate - 24/08/7");
  gtk_label_set_justify(GTK_LABEL(about_text), GTK_JUSTIFY_CENTER);

  gtk_grid_attach(GTK_GRID(grid), about_text, 0, 0, 1, 1);

  gtk_container_add(GTK_CONTAINER(about_window), grid);
  return about_window;
}

static GtkWidget* menu_bar_help_item() {
  GtkWidget* help_item = hgtk_menu_item_with_submenu_init("Help");

  GtkWidget* about_window = about_window_init();
  GtkWidget* about_item = gtk_menu_item_new_with_label("About");
  g_signal_connect(about_item, "activate", G_CALLBACK(on_window_activate), about_window);
  g_signal_connect(about_window, "delete-event", G_CALLBACK(on_window_delete), NULL);

  GtkWidget* docs_item = gtk_menu_item_new_with_label("Docs");

  hgtk_menu_item_add_menu_item(help_item, about_item);
  hgtk_menu_item_add_menu_item(help_item, docs_item);

  return help_item;
}

/**************************************************/
/* Menu Bar */

GtkWidget* main_menu_bar_get(SharedData* shared_data) {
  GtkWidget* menu_bar = gtk_menu_bar_new();
  
  GtkWidget* file_item = menu_bar_file_item(shared_data);
  GtkWidget* help_item = menu_bar_help_item();
  GtkWidget* info_item = menu_bar_info_item(&shared_data->current_file);

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_item);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), info_item);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help_item);

  return menu_bar;
}
