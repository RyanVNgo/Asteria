#ifndef MAIN_OPTIONS_BAR_H
#define MAIN_OPTIONS_BAR_H

#include "gtk/gtk.h"
#include "shared_data.h"

void main_options_bar_reset_scale_factor();

GtkWidget* main_options_bar_get(SharedData* shared_data);

#endif
