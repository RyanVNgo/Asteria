#include "main_image_display.h"

#include "app_cfitsio_helper.h"
#include "gdk-pixbuf/gdk-pixbuf.h"

GtkWidget* main_image_display_get(fitsfile** current_file_ptr) {
  image_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  image = gtk_image_new();
  gtk_container_add(GTK_CONTAINER(image_box), image);
  return image_box;
}

void main_image_display_load_new_image(fitsfile** current_file_ptr) {
  int status = 0;
  int width = 0, height = 0, channels = 0;
  hcfitsio_get_image_dimensions(current_file_ptr, &width, &height, &channels, &status);
  if (status) return;
  
  float* img_data;
  guchar *pixbuf_data;
  int pixel_count = width * height * channels;

  hcfitsio_get_image_data(current_file_ptr, &img_data);
  hcfitsio_img_data_to_pixbuf_format(current_file_ptr, &img_data, &pixbuf_data, pixel_count);

  GdkPixbuf* pixbuf = gdk_pixbuf_new_from_data(
      pixbuf_data,
      GDK_COLORSPACE_RGB,
      FALSE,
      8,
      width,
      height,
      width * channels,
      NULL,
      NULL
  );

  if (!image) {
    g_free(image);
    image = NULL;
  }

  gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
  g_object_unref(pixbuf);
  return;
}
