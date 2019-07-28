#ifndef LIST_H
#define LIST_H

#include <gtk/gtk.h>
#include "wad.h"

typedef enum {
	NAME_COL = 0,
	SIZE_COL,
	OFFSET_COL,
	NUM_COLS
}column_type;

GtkWidget* create_list(void);
void add_item(GtkWidget *list, const gchar *name, int size, int offset);
void list_clear(GtkWidget *list);
void list_from_wad(GtkWidget *list, Wad *wad);

#endif
