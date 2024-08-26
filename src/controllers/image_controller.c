/* primary header include */
#include "image_controller.h"

/* project files */
#include "../utils/fits_utils.h"
#include "../utils/display_img_utils.h"
#include <string.h>

void load_new_image(SharedData* shared_data) {
  if (!shared_data->current_file) return;
  
  int dim_count = h_fits_img_dim_count(shared_data->current_file);
  long dim_size[dim_count];
  h_fits_img_dim_size(shared_data->current_file, dim_size);
  
  int pixel_count = h_fits_img_pxl_count(shared_data->current_file);

  float* temp_float_data; 
  h_get_fits_img_data(shared_data->current_file, &temp_float_data);

  if (shared_data->fits_data) free(shared_data->fits_data);
  shared_data->fits_data = (uint16_t*)malloc(sizeof(uint16_t) * pixel_count);
  h_float_to_uint16_array(&temp_float_data, &shared_data->fits_data, pixel_count);
  
  uint16_t* temp_uint16_data = malloc(sizeof(uint16_t) * pixel_count);
  memcpy(temp_uint16_data, shared_data->fits_data, sizeof(uint16_t) * pixel_count);
  h_scale_fits_data(shared_data->thread_pool, &temp_uint16_data, pixel_count, dim_count, shared_data->preview_mode);

  guchar* pixbuf_data = (guchar*)malloc(sizeof(guchar) * pixel_count);
  h_uint16_to_pixbuf_format(&temp_uint16_data, &pixbuf_data, pixel_count);
  
  GdkPixbuf* base_pixbuf = gdk_pixbuf_new_from_data(
      pixbuf_data,
      GDK_COLORSPACE_RGB,
      FALSE,
      8,
      dim_size[0],
      dim_size[1],
      dim_size[0] * dim_size[2],
      h_destroy_pixbuf_data,
      NULL
  );

  if (shared_data->unscaled_pixbuf) {
    g_object_unref(shared_data->unscaled_pixbuf);
  }

  shared_data->unscaled_pixbuf = base_pixbuf;
  shared_data->display_scale = 1.0;
  h_display_img_adj_scale(shared_data->display_image, shared_data->unscaled_pixbuf, shared_data->display_scale);

  free(temp_float_data);
  free(temp_uint16_data);
  return;
}

#include "../core/bench_timer.h"
void update_image(SharedData* shared_data) {
  BenchTimer* bench_timer;
  bench_timer_start(&bench_timer, "Update_image");

  if (!shared_data->current_file) return;

  int dim_count = h_fits_img_dim_count(shared_data->current_file);
  bench_timer_interval(bench_timer, "dim_count");
  
  long dim_size[dim_count];
  h_fits_img_dim_size(shared_data->current_file, dim_size);
  bench_timer_interval(bench_timer, "dim_size");
  
  int pixel_count = h_fits_img_pxl_count(shared_data->current_file);
  bench_timer_interval(bench_timer, "pxl_count");

  uint16_t* temp_data = malloc(sizeof(uint16_t) * pixel_count);
  memcpy(temp_data, shared_data->fits_data, sizeof(uint16_t) * pixel_count);
  h_scale_fits_data(shared_data->thread_pool, &temp_data, pixel_count, dim_count, shared_data->preview_mode);
  bench_timer_interval(bench_timer, "scale_data");

  guchar* pixbuf_data = (guchar*)malloc(sizeof(guchar) * pixel_count);
  h_uint16_to_pixbuf_format(&temp_data, &pixbuf_data, pixel_count);
  bench_timer_interval(bench_timer, "uint16_to_pixbuf");

  GdkPixbuf* new_pixbuf = gdk_pixbuf_new_from_data(
      pixbuf_data,
      GDK_COLORSPACE_RGB,
      FALSE,
      8,
      dim_size[0],
      dim_size[1],
      dim_size[0] * dim_size[2],
      h_destroy_pixbuf_data,
      NULL
  );

  if (shared_data->unscaled_pixbuf) {
    g_object_unref(shared_data->unscaled_pixbuf);
  }
  
  shared_data->unscaled_pixbuf = new_pixbuf;
  h_display_img_adj_scale(shared_data->display_image, shared_data->unscaled_pixbuf, shared_data->display_scale);

  bench_timer_interval(bench_timer, "pixbuf_adj_scale");

  free(temp_data);
  return;
}

#define MAX_SCALE 4.00
#define MIN_SCALE 0.25

void image_scale_increase(SharedData* shared_data) {
  if (!shared_data->current_file) return;
  if (shared_data->display_scale >= MAX_SCALE) return;
  shared_data->display_scale *= 2.0;
  h_display_img_adj_scale(shared_data->display_image, shared_data->unscaled_pixbuf, shared_data->display_scale);
  return;
}

void image_scale_default(SharedData* shared_data) {
  if (!shared_data->current_file) return;
  if (shared_data->display_scale == 1.0) return;
  shared_data->display_scale = 1.0;
  h_display_img_adj_scale(shared_data->display_image, shared_data->unscaled_pixbuf, shared_data->display_scale);
}

void image_scale_decrease(SharedData* shared_data) {
  if (!shared_data->current_file) return;
  if (shared_data->display_scale <= MIN_SCALE) return;
  shared_data->display_scale /= 2.0;
  h_display_img_adj_scale(shared_data->display_image, shared_data->unscaled_pixbuf, shared_data->display_scale);
  return;
}
