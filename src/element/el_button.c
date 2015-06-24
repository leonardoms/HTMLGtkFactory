

#include "HTMLGtkFactory.h"

GtkWidget*
el_button(factory_element_t* el) {

	GtkWidget *btn;

	g_return_val_if_fail( el->gumbo_node->type = GUMBO_NODE_ELEMENT, NULL );

	btn = GTK_WIDGET(gtk_button_new());

	el->widget = btn;
	el->orig_widget = el->widget;

	return btn;
}
