#include "globals.h"
#include "menu.h"
#include "callbacks.h"
#include "list.h"
#include "dialog.h"

Wad *wad = NULL;

int main(int argc, char *argv[]) {
  GtkWidget *window;
  GtkWidget *menubar;
  GtkWidget *wad_submenu;
  GtkWidget *list_popup_menu;
  GtkWidget *box;

  wad = (Wad*)calloc(1, sizeof(Wad));

  gtk_init(&argc, &argv);

  list_popup_menu = menu_create(9, "Rename",  G_CALLBACK(rename_files_event),  NULL,
		  						   "Delete",  G_CALLBACK(del_files_event), 	   NULL,
		  				   		   "Extract", G_CALLBACK(extract_files_event), NULL);

  wad_submenu = menu_create(9, "New",  G_CALLBACK(wad_new_event),  NULL,
		  					   "Open", G_CALLBACK(wad_open_event), NULL,
							   "Save", G_CALLBACK(wad_save_event), NULL);

  menubar = menubar_create(15, "Wad",       NULL,                         wad_submenu,
		  					   "Add Files", G_CALLBACK(add_files_event),  NULL,
							   "Up",        G_CALLBACK(up_files_event),   NULL,
							   "Down",      G_CALLBACK(down_files_event), NULL,
							   "Quit",      G_CALLBACK(gtk_main_quit),    NULL);

  list = create_list();
  g_signal_connect_swapped(list, "button-press-event", G_CALLBACK(list_popup_show), list_popup_menu);

  box = gtk_vbox_new(FALSE, 3);
  gtk_box_pack_start(GTK_BOX(box), menubar, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), list, TRUE, TRUE, 0);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_container_add(GTK_CONTAINER(window), box);

  gtk_window_set_title(GTK_WINDOW(window), "WAD Tool");
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);  

  gtk_widget_show_all(window);

  gtk_main();
  if(wad) wad_free(wad);
  return 0;
}
