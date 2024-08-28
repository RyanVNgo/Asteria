#ifndef DATA_CONVERSION_H
#define DATA_CONVERSION_H

/* external libraries */
#include <stdint.h>

void h_float_to_uint16(float** float_data, uint16_t** uint16_buff, int pixel_count);

void h_uint16_to_uint8_format(uint16_t** uint16_data, uint8_t** pixbuf_buff, int pixel_count);

#endif
