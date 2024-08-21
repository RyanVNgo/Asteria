#ifndef IMAGE_CONTROLLER_H
#define IMAGE_CONTROLLER_H

/* external libraries */
#include <gtk/gtk.h>

/* project files */
#include "../core/shared_data.h"

void load_new_image(SharedData* shared_data);
void update_image(SharedData* shared_data);

void image_scale_increase(SharedData* shared_data);
void image_scale_decrease(SharedData* shared_data);

#endif
