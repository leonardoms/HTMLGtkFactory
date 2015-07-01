
#include <HTMLGtkFactory.h>

GtkWidget*
el_text(htmlgtk_element_t* el) {

	GtkWidget *text, *container;

	g_return_val_if_fail( el->gumbo_node->type = GUMBO_NODE_TEXT, NULL );

	text = GTK_WIDGET(gtk_label_new(el->gumbo_node->v.text.text));

	el->widget = text;
	el->orig_widget = el->widget;

	return text;
}
