
#include <HTMLGtkFactory.h>

GtkWidget*
el_body(htmlgtk_element_t* el) {

	GtkWidget* body = GTK_WIDGET(gtk_box_new(GTK_ORIENTATION_VERTICAL, 5));

	gtk_box_set_homogeneous( GTK_BOX(body), FALSE );

	el->widget = body;
	el->orig_widget = el->widget;

	return body;
}
