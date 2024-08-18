#ifndef APP_GTK_HELPER_H
#define APP_GTK_HELPER_H

/* external libraries */
#include <gtk/gtk.h>

GtkWidget* hgtk_menu_item_with_submenu_init(const char* label);

void hgtk_menu_item_add_menu_item(GtkWidget* menu_item, GtkWidget* item);

#endif
