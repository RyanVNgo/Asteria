#include <stdio.h>
#include "fitsio.h"

int communicateStatusAndExit(int *status) {
  fits_report_error(stderr, *status);
  exit(*status);
}

int main(int argc, char* argv[]) {
  
  fitsfile* fptr;
  char card[FLEN_CARD];
  int status = 0, nkeys, i;

  if (!argv[1]) return 1; // temporary check for if file name is provided, must properly implement later

  // test for cfitsio library installation
  fits_open_file(&fptr, argv[1], READONLY, &status);
  fits_get_hdrspace(fptr, &nkeys, NULL, &status);
  if (status) communicateStatusAndExit(&status);
  for (i = 1; i <= nkeys; i++) {
    fits_read_record(fptr, i, card, &status);
    printf("%s\n", card);
  }
  printf("END\n\n");
  fits_close_file(fptr, &status);
  if (status) communicateStatusAndExit(&status);

  return status;
}
