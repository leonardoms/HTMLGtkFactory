

#include "HTMLGtkFactory.h"

void
el_table_post_create(factory_element_t* el) {
	HTMLGtkFactory_newline(HTMLGtkFactory_get_el_body()); //TODO: change to parent master container (div, body, frame...).
}


GtkWidget*
el_table(factory_element_t* el) {
	GtkWidget *table;

	g_return_val_if_fail( el->gumbo_node->type = GUMBO_NODE_ELEMENT, NULL );

	table = GTK_WIDGET(gtk_grid_new());

	g_object_set_data( G_OBJECT(table), "last_pos", g_malloc0(sizeof(guint)*2) ); // guint row, col;

	el->widget = table;
	el->orig_widget = el->widget;

	el->post_create = el_table_post_create;

	return table;
}
