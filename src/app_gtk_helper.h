#ifndef MENU_FACTORY_H
#define MENU_FACTORY_H

#include <gtk/gtk.h>

GtkWidget* hgtk_menu_item_with_submenu_init(const char* label);

void hgtk_menu_item_add_menu_item(GtkWidget* menu_item, GtkWidget* item);

#endif
