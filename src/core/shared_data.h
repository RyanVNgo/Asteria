#ifndef SHARED_DATA_H
#define SHARED_DATA_H

/* external libraries */
#include <fitsio.h>
#include <gtk/gtk.h>

/* project files */
#include "threads.h"

enum PreviewMode {
  LINEAR,
  SQUARE_ROOT,
  AUTOSTRETCH
};

typedef struct SharedData {
  ThreadPool* thread_pool;

  fitsfile* current_file;
  uint16_t* fits_data;

  GtkWidget* display_image;
  GdkPixbuf* unscaled_pixbuf;
  float display_scale;
  enum PreviewMode preview_mode;

} SharedData;

#endif
