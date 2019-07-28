#ifndef MENU_H
#define MENU_H

#include <stdarg.h>
#include <gtk/gtk.h>

GtkWidget* menu_item_create(char *name, GCallback callback, GtkWidget *submenu);
GtkWidget* menu_create(int args, ...);
GtkWidget* menubar_create(int args, ...);

#endif
