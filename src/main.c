#include <stdio.h>
#include <unistd.h>
#include "fitsio.h"

int communicateStatusAndExit(int *status) {
  fits_report_error(stderr, *status);
  exit(*status);
}

void printHeader(fitsfile*);


int main(int argc, char* argv[]) {
  fitsfile* fptr;
  int status = 0;
  if (!argv[1]) return 1; // temporary check for if file name is provided, must properly implement later
  fits_open_file(&fptr, argv[1], READONLY, &status);

  int c;
  while ((c = getopt(argc, argv, "h")) != -1) {
    switch (c) {
      case 'h':
        printHeader(fptr);
        break;
    }
  }

  return status;
}

void printHeader(fitsfile* fptr) {
  char card[FLEN_CARD];
  int status = 0, nkeys, i;
  fits_get_hdrspace(fptr, &nkeys, NULL, &status);
  if (status) communicateStatusAndExit(&status);
  for (i = 1; i <= nkeys; i++) {
    fits_read_record(fptr, i, card, &status);
    printf("%s\n", card);
  }
  printf("END\n\n");
  fits_close_file(fptr, &status);
  if (status) communicateStatusAndExit(&status);
  return;
}
