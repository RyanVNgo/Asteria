#include "main_options_bar.h"

#include "glibconfig.h"
#include "gtk/gtk.h"
#include "main_image_display.h"

static float scale_factor;

void update_scale_label(GtkWidget* button, GtkWidget* label) {
  char* new_label = g_strdup_printf("%.2fx", scale_factor);
  gtk_label_set_label(GTK_LABEL(label), new_label);
  return;
}

GtkWidget* main_options_bar_get() {
  scale_factor = 1.0;

  guint spacing = 5;
  GtkWidget* options_bar = gtk_grid_new();
  gtk_container_set_border_width(GTK_CONTAINER(options_bar), spacing);

  GtkWidget* button_box = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
  
  GtkWidget* scale_label = gtk_label_new("1.00x");
  gtk_widget_set_size_request(scale_label, 20, 20);
  
  GtkWidget* scale_dec_button = gtk_button_new_with_label("-");
  gtk_widget_set_size_request(scale_dec_button, 20, 20);
  gtk_widget_set_can_focus(scale_dec_button, FALSE);
  g_signal_connect(scale_dec_button, "clicked", G_CALLBACK(main_image_display_dec_image_scale), &scale_factor);
  g_signal_connect(scale_dec_button, "clicked", G_CALLBACK(update_scale_label), scale_label);
  
  GtkWidget* scale_inc_button = gtk_button_new_with_label("+");
  gtk_widget_set_size_request(scale_inc_button, 20, 20);
  gtk_widget_set_can_focus(scale_inc_button, FALSE);
  g_signal_connect(scale_inc_button, "clicked", G_CALLBACK(main_image_display_inc_image_scale), &scale_factor);
  g_signal_connect(scale_inc_button, "clicked", G_CALLBACK(update_scale_label), scale_label);

  gtk_container_add(GTK_CONTAINER(button_box), scale_inc_button);
  gtk_container_add(GTK_CONTAINER(button_box), scale_label);
  gtk_container_add(GTK_CONTAINER(button_box), scale_dec_button);

  gtk_grid_attach(GTK_GRID(options_bar), button_box, 0, 0, 1, 1);

  gtk_grid_set_row_spacing(GTK_GRID(options_bar), spacing);
  gtk_grid_set_column_spacing(GTK_GRID(options_bar), spacing);

  return options_bar;
}
