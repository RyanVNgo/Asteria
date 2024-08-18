/* primary header include */
#include "main_options_bar.h"

/* project files */
#include "main_image_display.h"

static float scale_factor;

void update_scale_label(GtkWidget* button, GtkWidget* label) {
  char* new_label = g_strdup_printf("%.2fx", scale_factor);
  gtk_label_set_label(GTK_LABEL(label), new_label);
  return;
}

void main_options_bar_reset_scale_factor() {
  scale_factor = 1.0;
  return;
}

void dec_image_scale(GtkWidget* widget, SharedData* shared_data) {
  main_image_display_dec_image_scale(&scale_factor);
  return;
}

void inc_image_scale(GtkWidget* widget, SharedData* shared_data) {
  main_image_display_inc_image_scale(&scale_factor);
  return;
}

GtkWidget* scale_widgets_get(SharedData* shared_data) {
  GtkWidget* button_box = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
  
  GtkWidget* scale_label = gtk_label_new("1.00x");
  gtk_widget_set_size_request(scale_label, 20, 20);

  GtkWidget* scale_dec_button = gtk_button_new_with_label("-");
  gtk_widget_set_size_request(scale_dec_button, 20, 20);
  gtk_widget_set_can_focus(scale_dec_button, FALSE);
  g_signal_connect(scale_dec_button, "clicked", G_CALLBACK(dec_image_scale), shared_data);
  g_signal_connect(scale_dec_button, "clicked", G_CALLBACK(update_scale_label), scale_label);
  
  GtkWidget* scale_inc_button = gtk_button_new_with_label("+");
  gtk_widget_set_size_request(scale_inc_button, 20, 20);
  gtk_widget_set_can_focus(scale_inc_button, FALSE);
  g_signal_connect(scale_inc_button, "clicked", G_CALLBACK(inc_image_scale), shared_data);
  g_signal_connect(scale_inc_button, "clicked", G_CALLBACK(update_scale_label), scale_label);

  gtk_container_add(GTK_CONTAINER(button_box), scale_inc_button);
  gtk_container_add(GTK_CONTAINER(button_box), scale_label);
  gtk_container_add(GTK_CONTAINER(button_box), scale_dec_button);

  return button_box;
}

void change_preview_mode_label(GtkWidget* set_widget, gpointer preview_mode_menu_button) {
  const gchar* label = gtk_menu_item_get_label(GTK_MENU_ITEM(set_widget));

  if (!strcmp(label, "Linear")) {
    gtk_button_set_label(GTK_BUTTON(preview_mode_menu_button), "Linear");
    main_image_display_set_preview_mode(LINEAR);
  } else if (!strcmp(label, "Square Root")) {
    gtk_button_set_label(GTK_BUTTON(preview_mode_menu_button), "Square Root");
    main_image_display_set_preview_mode(SQUARE_ROOT);
  } else if (!strcmp(label, "Autostretch")) {
    gtk_button_set_label(GTK_BUTTON(preview_mode_menu_button), "Autostretch");
    main_image_display_set_preview_mode(AUTOSTRETCH);
  }

  return;
}

void update_image_preview(GtkWidget* set_widget, SharedData* shared_data) {
  main_image_display_load_new_image(shared_data->thread_pool, &shared_data->current_file);
  return;
}

GtkWidget* preview_mode_widgets_get(SharedData* shared_data) {
  GtkWidget* preview_mode_menu_button = gtk_menu_button_new();
  gtk_widget_set_size_request(preview_mode_menu_button, 120, 20);
  gtk_widget_set_can_focus(preview_mode_menu_button, FALSE);
  gtk_button_set_label(GTK_BUTTON(preview_mode_menu_button), "Linear");

  GtkWidget* preview_mode_menu = gtk_menu_new();

  GtkWidget* set_linear = gtk_menu_item_new_with_label("Linear");
  g_signal_connect(set_linear, "activate", G_CALLBACK(change_preview_mode_label), preview_mode_menu_button);
  g_signal_connect(set_linear, "activate", G_CALLBACK(update_image_preview), shared_data);

  GtkWidget* set_square_root = gtk_menu_item_new_with_label("Square Root");
  g_signal_connect(set_square_root, "activate", G_CALLBACK(change_preview_mode_label), preview_mode_menu_button);
  g_signal_connect(set_square_root, "activate", G_CALLBACK(update_image_preview), shared_data);

  GtkWidget* set_autostretch = gtk_menu_item_new_with_label("Autostretch");
  g_signal_connect(set_autostretch, "activate", G_CALLBACK(change_preview_mode_label), preview_mode_menu_button);
  g_signal_connect(set_autostretch, "activate", G_CALLBACK(update_image_preview), shared_data);

  gtk_menu_shell_append(GTK_MENU_SHELL(preview_mode_menu), set_linear);
  gtk_menu_shell_append(GTK_MENU_SHELL(preview_mode_menu), set_square_root);
  gtk_menu_shell_append(GTK_MENU_SHELL(preview_mode_menu), set_autostretch);

  gtk_widget_show_all(preview_mode_menu);
  gtk_menu_button_set_popup(GTK_MENU_BUTTON(preview_mode_menu_button), preview_mode_menu);

  return preview_mode_menu_button;
}

GtkWidget* main_options_bar_get(SharedData* shared_data) {
  scale_factor = 1.0;

  guint spacing = 5;
  GtkWidget* options_bar = gtk_grid_new();
  gtk_container_set_border_width(GTK_CONTAINER(options_bar), spacing);

  GtkWidget* scale_widgets = scale_widgets_get(shared_data);
  gtk_grid_attach(GTK_GRID(options_bar), scale_widgets, 0, 0, 1, 1);

  GtkWidget* preview_mode_widgets = preview_mode_widgets_get(shared_data);
  gtk_grid_attach(GTK_GRID(options_bar), preview_mode_widgets, 1, 0, 1, 1);

  gtk_grid_set_row_spacing(GTK_GRID(options_bar), spacing);
  gtk_grid_set_column_spacing(GTK_GRID(options_bar), spacing);

  return options_bar;
}
