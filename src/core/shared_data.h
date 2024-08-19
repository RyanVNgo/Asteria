#ifndef SHARED_DATA_H
#define SHARED_DATA_H

/* external libraries */
#include <fitsio.h>
#include <gtk/gtk.h>

/* project files */
#include "gdk-pixbuf/gdk-pixbuf.h"
#include "threads.h"

typedef struct SharedData {
  ThreadPool* thread_pool;
  fitsfile* current_file;
  GtkWidget* display_image;
  GdkPixbuf* unscaled_pixbuf;
  float display_scale;
} SharedData;

#endif
