#ifndef MAIN_MENU_BAR_H
#define MAIN_MENU_BAR_H

#include <gtk/gtk.h>
#include "fitsio.h"

GtkWidget* main_menu_bar_get(fitsfile** current_file);

#endif
