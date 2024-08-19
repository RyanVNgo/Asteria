#ifndef IMAGE_CONTROLLER_H
#define IMAGE_CONTROLLER_H

/* external libraries */
#include <gtk/gtk.h>

/* project files */
#include "../core/shared_data.h"

void load_new_image(SharedData* shared_data);

void set_default_scale(SharedData* shared_data);

#endif
