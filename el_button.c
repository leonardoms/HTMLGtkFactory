

#include "HTMLGtkFactory.h"

GtkWidget*
el_button(factory_element_t* el) {

	GtkWidget *btn, *container;

	g_print("button");

	//g_return_val_if_fail( el->gumbo_node->type = GUMBO_NODE_ELEMENT, NULL );

	btn = GTK_WIDGET(gtk_button_new());

	el->widget = btn;

	return btn;
}
