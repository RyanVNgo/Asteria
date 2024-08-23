/* primary header include */
#include "image_controller.h"

/* project files */
#include "../utils/fits_utils.h"
#include "../utils/display_img_utils.h"

void load_new_image(SharedData* shared_data) {
  if (!shared_data->current_file) return;
  
  int dim_count = h_fits_img_dim_count(shared_data->current_file);
  long dim_size[dim_count];
  h_fits_img_dim_size(shared_data->current_file, dim_size);
  
  int pixel_count = h_fits_img_pxl_count(shared_data->current_file);

  float* img_data; 
  h_get_fits_img_data(shared_data->current_file, &img_data);

  h_scale_img_data(&img_data, pixel_count, dim_count, shared_data->preview_mode);

  guchar* pixbuf_data = (guchar*)malloc(sizeof(guchar) * pixel_count);
  h_fits_img_data_to_pixbuf_format(
      shared_data->current_file, 
      &img_data, 
      &pixbuf_data, 
      pixel_count
      );
  
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

  free(img_data);
  return;
}

#include <time.h>
void update_image(SharedData* shared_data) {
  if (!shared_data->current_file) return;

  int dim_count = h_fits_img_dim_count(shared_data->current_file);
  long dim_size[dim_count];
  h_fits_img_dim_size(shared_data->current_file, dim_size);
  
  int pixel_count = h_fits_img_pxl_count(shared_data->current_file);
  float* img_data;
  h_get_fits_img_data(shared_data->current_file, &img_data);

  h_scale_img_data(&img_data, pixel_count, dim_count, shared_data->preview_mode);

  guchar* pixbuf_data = (guchar*)malloc(sizeof(guchar) * pixel_count);
  h_fits_img_data_to_pixbuf_format(
      shared_data->current_file,
      &img_data,
      &pixbuf_data,
      pixel_count
      );

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

  //free(pixbuf_data);
  free(img_data);
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
