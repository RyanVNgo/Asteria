/* primary header include */
#include "data_conversion.h"

#include <limits.h>

void h_float_to_uint16(float** float_data, uint16_t** uint16_buff, int pixel_count) {
  for (int i = 0; i < pixel_count; i++) {
    (*uint16_buff)[i] = (uint16_t)((*float_data)[i] * UINT16_MAX);
  }
  return;
}

void h_uint16_to_uint8_format(uint16_t** uint16_data, uint8_t** pixbuf_buff, int pixel_count) {
  int channel_size = pixel_count / 3;
  for (int c = 0; c < 3; c++) {
    int i = c;
    for (int p = c * channel_size; p < (c + 1) * channel_size; p++) {
      (*pixbuf_buff)[i] = (*uint16_data)[p]  / UCHAR_MAX;
      if ((*pixbuf_buff)[i] > UCHAR_MAX) (*pixbuf_buff)[i] = UCHAR_MAX;
      i += 3;
    }
  }
  return;
}

