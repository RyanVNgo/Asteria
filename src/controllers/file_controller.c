/* primary header include */
#include "file_controller.h"

/* external libraries */
#include <gtk/gtk.h>

/* project files */
#include "../helpers/app_cfitsio_helper.h"

/********************************************************************************/
/* internal method definitions */
/* these methods are inaccessible to external users */

void open_file_dialog(char** filename_buffer_ptr) {
  /* create dialog widget */
  GtkWidget* dialog = gtk_file_chooser_dialog_new(
      "Open File",
      NULL,
      GTK_FILE_CHOOSER_ACTION_OPEN,
      "_Cancel", GTK_RESPONSE_CANCEL,
      "_Open", GTK_RESPONSE_ACCEPT,
      NULL
      );
  
  /* run dialog window */
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    *filename_buffer_ptr = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
  }
  
  /* release dialog widget */
  gtk_widget_destroy(dialog);
  return;
}

void save_as_file_dialog(char** filename_buffer_ptr) {
  /* create dialog widget */
  GtkWidget* dialog = gtk_file_chooser_dialog_new(
      "Save As",
      NULL,
      GTK_FILE_CHOOSER_ACTION_SAVE,
      "_Cancel", GTK_RESPONSE_CANCEL,
      "_Save", GTK_RESPONSE_ACCEPT,
      NULL
      );
  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);
  
  /* run doalog window */
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    *filename_buffer_ptr = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
  } 

  /* release dialog widget */
  gtk_widget_destroy(dialog);
  return;
}

/********************************************************************************/
/* external method definitions */
/* these methods are accessible to external users through file_controller.h */

void get_fitsfile(fitsfile** fitsfile_ptr) {
  char* fitsfile_absolute_path = NULL;
  open_file_dialog(&fitsfile_absolute_path);
  if (fitsfile_absolute_path) {
    h_open_fits_file(fitsfile_ptr, fitsfile_absolute_path);
  }
  free(fitsfile_absolute_path);
  return;
}

void save_as_fitsfile(fitsfile* fitsfile_ptr) {
  if (!fitsfile_ptr) return;
  char* fitsfile_absolute_path = NULL;
  save_as_file_dialog(&fitsfile_absolute_path);
  if (fitsfile_absolute_path) {
    h_save_as_fits_file(fitsfile_ptr, fitsfile_absolute_path);
  }
  free(fitsfile_absolute_path);
  return;
}

