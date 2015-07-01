
#include <HTMLGtkFactory.h>

GtkWidget*
el_input_text(htmlgtk_element_t* el) {

	GtkWidget *entry;

	g_return_val_if_fail( el->gumbo_node->type = GUMBO_NODE_TEXT, NULL );

	entry = GTK_WIDGET(gtk_entry_new());

	el->widget = entry;
	el->orig_widget = el->widget;

	return entry;
}
