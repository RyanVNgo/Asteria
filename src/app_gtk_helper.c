#include "app_gtk_helper.h"
#include "gtk/gtk.h"

GtkWidget* hgtk_menu_item_with_submenu_init(const char* label) {
  GtkWidget* new_menu_item = gtk_menu_item_new_with_label(label);
  GtkWidget* new_menu_item_submenu = gtk_menu_new();
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(new_menu_item), new_menu_item_submenu);
  return new_menu_item;
}

void hgtk_menu_item_add_menu_item(GtkWidget* menu_item, GtkWidget* item) {
  GtkWidget* menu_item_submenu = gtk_menu_item_get_submenu(GTK_MENU_ITEM(menu_item));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_item_submenu), item);
}

