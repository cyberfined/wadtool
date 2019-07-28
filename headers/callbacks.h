#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "dialog.h"



void wad_open_event(GtkWidget *w, GdkEvent *e);
void wad_save_event(GtkWidget *w, GdkEvent *e);
void wad_new_event(GtkWidget *w, GdkEvent *e);
void add_files_event(GtkWidget *w, GdkEvent *e);

gboolean delete_selected(GtkTreeModel  *model,
						 GtkTreePath   *path,
                         GtkTreeIter   *iter,
                         gpointer       userdata);

gboolean extract_selected(GtkTreeModel  *model,
						  GtkTreePath   *path,
                          GtkTreeIter   *iter,
                          gpointer       userdata);

gboolean rename_selected(GtkTreeModel  *model,
						 GtkTreePath   *path,
                         GtkTreeIter   *iter,
                         gpointer       userdata);

gboolean up_selected(GtkTreeModel  		*model,
						  GtkTreePath   *path,
                          GtkTreeIter   *iter,
                          gpointer       userdata);

gboolean down_selected(GtkTreeModel    *model,
						 GtkTreePath   *path,
                         GtkTreeIter   *iter,
                         gpointer       userdata);

void rename_files_event(GtkWidget *w, GdkEvent *e);
void del_files_event(GtkWidget *w, GdkEvent *e);
void extract_files_event(GtkWidget *w, GdkEvent *e);
void up_files_event(GtkWidget *w, GdkEvent *e);
void down_files_event(GtkWidget *w, GdkEvent *e);
int list_popup_show(GtkWidget *w, GdkEvent *e);

#endif
