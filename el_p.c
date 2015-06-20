

#include "HTMLGtkFactory.h"

GtkWidget*
el_p(factory_element_t* el) {
	GtkWidget *p;

	g_return_val_if_fail( el->gumbo_node->type = GUMBO_NODE_ELEMENT, NULL );

	p = GTK_WIDGET(gtk_box_new(GTK_ORIENTATION_VERTICAL, 5));

	el->widget = p;

	return p;
}