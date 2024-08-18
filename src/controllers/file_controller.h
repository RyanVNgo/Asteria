#ifndef FILE_CONTROLLER_H
#define FILE_CONTROLLER_H

/* external liraries */
#include "fitsio.h"

void get_fitsfile(fitsfile** fitsfile_ptr);
void save_as_fitsfile(fitsfile* fitsfile_ptr);


#endif
