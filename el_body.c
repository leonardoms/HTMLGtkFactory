
#include "HTMLGtkFactory.h"

GtkWidget*
el_body(factory_element_t* el) {

	g_print("body");

	GtkWidget* body = GTK_WIDGET(gtk_box_new(GTK_ORIENTATION_VERTICAL, 5));

	gtk_box_set_homogeneous( GTK_BOX(body), FALSE );

	el->widget = body;

	return body;
}
