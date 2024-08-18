#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include "fitsio.h"
#include "threads.h"

typedef struct SharedData {
  ThreadPool* thread_pool;
  fitsfile* current_file;
} SharedData;

#endif
