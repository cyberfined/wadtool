#include "dialog.h"

char* file_dialog_create(char *title, GtkFileChooserAction action) {
	char *result = NULL;
	GtkWidget *dialog = gtk_file_chooser_dialog_new(title,
													NULL,
													action,
													"Cancel", GTK_RESPONSE_CANCEL,
													"OK", GTK_RESPONSE_OK,
													NULL);
	if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
		result = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
	}
	gtk_widget_destroy(dialog);
	return result;
}

GList* multiselect_file_dialog_create(char *title) {
	GList *result = NULL;
	GtkWidget *dialog = gtk_file_chooser_dialog_new(title,
													NULL,
													GTK_FILE_CHOOSER_ACTION_OPEN,
													"Cancel", GTK_RESPONSE_CANCEL,
													"OK", GTK_RESPONSE_OK,
													NULL);
	gtk_file_chooser_set_select_multiple(GTK_DIALOG(dialog), TRUE);
	if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
		result = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(dialog));
	}
	gtk_widget_destroy(dialog);
	return result;
}

gchar* prompt_dialog_create(char *title) {
	GtkWidget *dialog;
	GtkWidget *box;
	GtkWidget *entry;
	GtkWidget *label;
	char *buf;
	char *res = NULL;

	entry = gtk_entry_new();
	label = gtk_label_new(title);

	dialog = gtk_dialog_new_with_buttons(title,
			                             NULL,
										 0,
										 GTK_STOCK_OK, GTK_RESPONSE_OK,
										 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
										 NULL);

	box = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	gtk_box_pack_end(GTK_BOX(box), entry, TRUE, TRUE, 0);
	gtk_box_pack_end(GTK_BOX(box), label, TRUE, TRUE, 0);
	gtk_widget_show_all(box);

	if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
		buf = gtk_entry_get_text(GTK_ENTRY(entry));
		res = strdup(buf);
	}

	gtk_widget_destroy(dialog);
	return res;
}
