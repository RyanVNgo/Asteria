#ifndef OPTIONS_BAR_H
#define OPTIONS_BAR_H

/* external libraries */
#include <gtk/gtk.h>

/* project files */
#include "../core/shared_data.h"

void options_bar_reset_scale_factor();

GtkWidget* options_bar_get(SharedData* shared_data);

#endif
