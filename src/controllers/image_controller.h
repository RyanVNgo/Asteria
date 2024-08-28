#ifndef IMAGE_CONTROLLER_H
#define IMAGE_CONTROLLER_H

/* project files */
#include "../core/shared_data.h"

#define MAX_DISPLAY_SCALE 4.00
#define MIN_DISPLAY_SCALE 0.25

void load_new_image(SharedData* shared_data);
void update_image(SharedData* shared_data);

void image_scale_increase(SharedData* shared_data);
void image_scale_default(SharedData* shared_data);
void image_scale_decrease(SharedData* shared_data);

void image_vertical_flip(SharedData* shared_data);
void image_horizontal_flip(SharedData* shared_data);

#endif
