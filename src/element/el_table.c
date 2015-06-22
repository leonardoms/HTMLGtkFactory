

#include "HTMLGtkFactory.h"

GtkWidget*
el_table(factory_element_t* el) {
	GtkWidget *table;

	g_return_val_if_fail( el->gumbo_node->type = GUMBO_NODE_ELEMENT, NULL );

	table = GTK_WIDGET(gtk_grid_new());

	g_object_set_data( G_OBJECT(table), "last_pos", g_malloc0(sizeof(guint)*2) ); // guint row, col;

	el->widget = table;

	return table;
}
