#include "app_glib_helper.h"

char* hglib_get_absolute_path() {
  char* file_absolute_path = NULL;
  GtkWidget* dialog = gtk_file_chooser_dialog_new(
      "Open File",
      NULL,
      GTK_FILE_CHOOSER_ACTION_OPEN,
      "_Cancel", GTK_RESPONSE_CANCEL,
      "_Open", GTK_RESPONSE_ACCEPT,
      NULL
      );
  
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    file_absolute_path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
  }

  gtk_widget_destroy(dialog);
  return file_absolute_path;
}

char* hglib_saveas_absolute_path() {
  char* saveas_filepath = NULL;
  GtkWidget* dialog = gtk_file_chooser_dialog_new(
      "Save as",
      NULL,
      GTK_FILE_CHOOSER_ACTION_SAVE,
      "_Cancel", GTK_RESPONSE_CANCEL,
      "_Save", GTK_RESPONSE_ACCEPT,
      NULL
      );
  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);
  
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    saveas_filepath = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
  }

  gtk_widget_destroy(dialog);
  return saveas_filepath;
}
