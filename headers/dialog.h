#ifndef DIALOG_H
#define DIALOG_H

#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

char* file_dialog_create(char *title, GtkFileChooserAction action);
GList* multiselect_file_dialog_create(char *title);
char* prompt_dialog_create(char *title);

#endif
