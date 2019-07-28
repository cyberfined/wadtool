#include "menu.h"

GtkWidget* menu_item_create(char *name, GCallback callback, GtkWidget *submenu) {
	GtkWidget *item;
	item = gtk_menu_item_new_with_label(name);
	if(submenu) gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), submenu);
	if(callback) g_signal_connect_swapped(item,
										  "activate",
										  callback,
										  NULL);
	gtk_widget_show(item);
	return item;
}

GtkWidget* menu_create(int args, ...) {
	GtkWidget *menu;
	GtkWidget *item;
	va_list ap;

	char *name;
	GCallback callback;
	GtkWidget *submenu;

	int i;
	
	if(args % 3 != 0) return NULL;

	menu = gtk_menu_new();

	va_start(ap, args);

	for(i = 0; i < args; i+=3) {
		name = va_arg(ap, char*);
		callback = va_arg(ap, GCallback);
		submenu = va_arg(ap, GtkWidget*);
		item = menu_item_create(name, callback, submenu);
  		gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	}

	va_end(ap);
	return menu;
}

GtkWidget* menubar_create(int args, ...) {
	GtkWidget *menubar;
	GtkWidget *item;
	va_list ap;

	char *name;
	GCallback callback;
	GtkWidget *submenu;

	int i;

	if(args % 3 != 0) return NULL;

	menubar = gtk_menu_bar_new();

	va_start(ap, args);
	
	for(i = 0; i < args; i+=3) {
		name = va_arg(ap, char*);
		callback = va_arg(ap, GCallback);
		submenu = va_arg(ap, GtkWidget*);
		item = menu_item_create(name, callback, submenu);
		gtk_menu_bar_append(GTK_MENU_BAR(menubar), item);
	}

	va_end(ap);
	return menubar;
}
