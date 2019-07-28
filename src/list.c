#include "list.h"

GtkWidget* create_list(void) {
	GtkWidget *list;
	GtkListStore *store;
	GtkCellRenderer *cell;
	GtkTreeViewColumn *column;

	list = gtk_tree_view_new();
	gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(list)),
								GTK_SELECTION_MULTIPLE);

	store = gtk_list_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_UINT, G_TYPE_INT);

	cell = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("Name",
													  cell,
													  "text", NAME_COL,
													  NULL);
	gtk_tree_view_column_set_expand(column, TRUE);	
	gtk_tree_view_insert_column(GTK_TREE_VIEW(list), column, -1);

	cell = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("Size",
													  cell,
													  "text", SIZE_COL,
													  NULL);

	gtk_tree_view_column_set_expand(column, TRUE);	
	gtk_tree_view_insert_column(GTK_TREE_VIEW(list), column, -1);

	cell = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("Offset",
													  cell,
													  "text", OFFSET_COL,
													  NULL);

	gtk_tree_view_column_set_expand(column, TRUE);	
	gtk_tree_view_insert_column(GTK_TREE_VIEW(list), column, -1);

	gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));
	g_object_unref(store);

	return list;
}

void add_item(GtkWidget *list, const gchar *name, int size, int offset) {
	GtkListStore *store;
	GtkTreeIter iter;
	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter,
					   NAME_COL, name,
					   SIZE_COL, size,
					   OFFSET_COL, offset,
					   -1);

}

void list_clear(GtkWidget *list) {
	GtkListStore *store;
	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
	gtk_list_store_clear(store);
}

void list_from_wad(GtkWidget *list, Wad *wad) {
	Lump *i;
	list_clear(list);
	for(i = wad->lumps; i != NULL; i = i->next) {
		add_item(list,
				 i->name,
				 i->size,
				 i->offset);
	}
}
