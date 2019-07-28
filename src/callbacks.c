#include "callbacks.h"

void wad_open_event(GtkWidget *w, GdkEvent *e) {
	char *wadname;
	GtkWidget *msg_dialog;
	wadname = file_dialog_create("Open WAD", GTK_FILE_CHOOSER_ACTION_OPEN); 
	if(wadname) {
		wad_free(wad);
		wad = wad_open(wadname);
		if(!wad) {
			msg_dialog = gtk_message_dialog_new(NULL,
					                            0,
												GTK_MESSAGE_WARNING,
												GTK_BUTTONS_OK,
												"Failed to open wad: %s", wadname,
												NULL);
			gtk_dialog_run(GTK_DIALOG(msg_dialog));
			gtk_widget_destroy(msg_dialog);

			wad = (Wad*)calloc(1, sizeof(Wad));
			list_clear(list);
			return;
		}
		list_from_wad(list, wad);
		free(wadname);
	}
}

void wad_save_event(GtkWidget *w, GdkEvent *e) {
	char *wadname = file_dialog_create("Save WAD", GTK_FILE_CHOOSER_ACTION_SAVE);
	if(wadname) {
		wad_save(wad, wadname);
		free(wadname);
	}
}

void wad_new_event(GtkWidget *w, GdkEvent *e) {
	wad_free(wad);
	wad = (Wad*)calloc(1, sizeof(Wad));
	list_clear(list);
}

void add_files_event(GtkWidget *w, GdkEvent *e) {
	GList *fnames = multiselect_file_dialog_create("Add Files");
	GList *i;
	if(fnames) {
		for(i = fnames; i != NULL; i = i->next) {
			wad_add_file(wad, (char*)i->data);
			g_free(i->data);
		}
		wad_calc_offsets(wad);
		list_from_wad(list, wad);
	}
}

gboolean delete_selected 	 (GtkTreeModel  *model,
                              GtkTreePath   *path,
                              GtkTreeIter   *iter,
                              gpointer       userdata) 
{
	char *name;
    gtk_tree_model_get (model, iter, 0, &name, -1);
	wad_del_file(wad, name);
}

gboolean extract_selected 	 (GtkTreeModel  *model,
                              GtkTreePath   *path,
                              GtkTreeIter   *iter,
                              gpointer       userdata) 
{
	char *name;
	char *dir = (char*)userdata;
	char *filepath; 
    gtk_tree_model_get (model, iter, 0, &name, -1);
	filepath = (char*)calloc(strlen(dir) + strlen(name) + 2, sizeof(char));
	strcat(filepath, dir);
	strcat(filepath, "/");
	strcat(filepath, name);
	wad_extract_lump(wad, name, filepath);
	free(filepath);
}

gboolean rename_selected 	 (GtkTreeModel  *model,
                              GtkTreePath   *path,
                              GtkTreeIter   *iter,
                              gpointer       userdata) 
{
	char *name;
	char *n_name;
    gtk_tree_model_get (model, iter, 0, &name, -1);
	n_name = prompt_dialog_create("Rename");
	if(n_name) {
		wad_rename_lump(wad, name, n_name);
		free(n_name);
	}
}

gboolean up_selected 	 (GtkTreeModel  *model,
                              GtkTreePath   *path,
                              GtkTreeIter   *iter,
                              gpointer       userdata) 
{
	char *name;
	int count = (int)userdata;
    gtk_tree_model_get (model, iter, 0, &name, -1);
	while(count--) wad_up_lump(wad, name);
}

gboolean down_selected 	 (GtkTreeModel  *model,
                              GtkTreePath   *path,
                              GtkTreeIter   *iter,
                              gpointer       userdata) 
{
	char *name;
	int count = (int)userdata;
    gtk_tree_model_get (model, iter, 0, &name, -1);
	while(count--) wad_down_lump(wad, name);
}

void rename_files_event(GtkWidget *w, GdkEvent *e) {
	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
	gtk_tree_selection_selected_foreach(selection, rename_selected, NULL);
	list_from_wad(list, wad);
}

void del_files_event(GtkWidget *w, GdkEvent *e) {
	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
	gtk_tree_selection_selected_foreach(selection, delete_selected, NULL);
	wad_calc_offsets(wad);
	list_from_wad(list, wad);
}

void extract_files_event(GtkWidget *w, GdkEvent *e) {
	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
	char *dir = file_dialog_create("Extract Files", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
	if(!dir) return;
	gtk_tree_selection_selected_foreach(selection, extract_selected, (gpointer)dir);
	free(dir);
}

void up_files_event(GtkWidget *w, GdkEvent *e) {
	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
	int count = gtk_tree_selection_count_selected_rows(selection);
	gtk_tree_selection_selected_foreach(selection, up_selected, (gpointer)count);
	wad_calc_offsets(wad);
	list_from_wad(list, wad);
}

void down_files_event(GtkWidget *w, GdkEvent *e) {
	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
	int count = gtk_tree_selection_count_selected_rows(selection);
	gtk_tree_selection_selected_foreach(selection, down_selected, (gpointer)count);
	wad_calc_offsets(wad);
	list_from_wad(list, wad);
}

int list_popup_show(GtkWidget *w, GdkEvent *e) {
	GdkEventButton *bevent = (GdkEventButton*)e;
	if(bevent->button == 3) {
		gtk_menu_popup(GTK_MENU(w), NULL, NULL, NULL, NULL,
					   bevent->button, bevent->time);
		return TRUE;
	}
	return FALSE;
}
