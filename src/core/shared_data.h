#ifndef SHARED_DATA_H
#define SHARED_DATA_H

/* external libraries */
#include <fitsio.h>

/* project files */
#include "threads.h"

typedef struct SharedData {
  ThreadPool* thread_pool;
  fitsfile* current_file;
} SharedData;

#endif
