#include "app_glib_helper.h"
#include "glib.h"

GFile* hglib_get_file(GtkWidget* widget, gpointer data) {
  GFile* file = NULL;
  GtkWidget* dialog = gtk_file_chooser_dialog_new(
      "Open File",
      NULL,
      GTK_FILE_CHOOSER_ACTION_OPEN,
      "_Cancel", GTK_RESPONSE_CANCEL,
      "_Open", GTK_RESPONSE_ACCEPT,
      NULL
      );
  
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
    file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(dialog));

  gtk_widget_destroy(dialog);
  return file;
}

