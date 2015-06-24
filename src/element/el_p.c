

#include "HTMLGtkFactory.h"

void
el_p_post_create(factory_element_t* el) {
	HTMLGtkFactory_newline(HTMLGtkFactory_get_el_body()); //TODO: change to parent master container (div, body, frame...).
}

GtkWidget*
el_p(factory_element_t* el) {
	GtkWidget *p;

	g_return_val_if_fail( el->gumbo_node->type = GUMBO_NODE_ELEMENT, NULL );

	p = GTK_WIDGET(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5));

	el->widget = p;
	el->orig_widget = el->widget;

	el->post_create = el_p_post_create;

	return p;
}
