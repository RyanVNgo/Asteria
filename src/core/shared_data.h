#ifndef SHARED_DATA_H
#define SHARED_DATA_H

/* external libraries */
#include <fitsio.h>
#include <gtk/gtk.h>

/* project files */
#include "threads.h"

typedef struct SharedData {
  ThreadPool* thread_pool;
  fitsfile* current_file;
  GtkWidget* display_image;
  float display_scale;
} SharedData;

#endif
