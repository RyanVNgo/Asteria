/* primary header include */
#include "image_controller.h"

/* project files */
#include "../helpers/app_cfitsio_helper.h"

/********************************************************************************/
/* internal method definitions */
/* these methods are inaccessible to external users */

void destroy_pixbuf_data(guchar* pixbuf_data, gpointer data) {
  free(pixbuf_data);
  return;
}

/********************************************************************************/
/* external method definitions */
/* these methods are accessible to external users through image_controller.h */

void load_new_image(SharedData* shared_data) {
  if (!shared_data->current_file) return;
  
  int dim_count = h_fits_img_dim_count(shared_data->current_file);
  long dim_size[dim_count];
  h_fits_img_dim_size(shared_data->current_file, dim_size);
  
  int pixel_count = h_fits_img_pxl_count(shared_data->current_file);

  float* img_data; 
  h_get_fits_img_data(&shared_data->current_file, &img_data);

  guchar* pixbuf_data = (guchar*)malloc(sizeof(guchar) * pixel_count);
  h_fits_img_data_to_pixbuf_format(
      &shared_data->current_file, 
      &img_data, 
      &pixbuf_data, 
      pixel_count, 
      0);
  
  GdkPixbuf* base_pixbuf = gdk_pixbuf_new_from_data(
      pixbuf_data,
      GDK_COLORSPACE_RGB,
      FALSE,
      8,
      dim_size[0],
      dim_size[1],
      dim_size[0] * dim_size[2],
      destroy_pixbuf_data,
      NULL
  );

  gtk_image_set_from_pixbuf(GTK_IMAGE(shared_data->display_image), base_pixbuf);

  g_object_unref(base_pixbuf);
  free(img_data);
  return;
}

