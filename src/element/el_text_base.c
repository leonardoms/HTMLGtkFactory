#include "HTMLGtkFactory.h"

GtkWidget*
el_text_base(factory_element_t* el) {
	GtkWidget *text_base;

	g_return_val_if_fail( el->gumbo_node->type = GUMBO_NODE_ELEMENT, NULL );

	text_base = GTK_WIDGET(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));

	el->widget = text_base;
	el->orig_widget = el->widget;

	return text_base;
}
