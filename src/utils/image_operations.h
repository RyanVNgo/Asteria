#ifndef IMAGE_OPERATIONS_H
#define IMAGE_OPERATIONS_H

/* project files */
#include "../core/shared_data.h"
#include "../core/threads.h"

void h_stretch_img_data(ThreadPool* thread_pool, uint16_t** fits_data, int pixel_count, int dim_count, enum PreviewMode preview_mode);

void h_vertical_flip_data(uint16_t** fits_data, long* dim_sizes);
void h_horizontal_flip_data(uint16_t** fits_data, long* dim_sizes);

#endif
