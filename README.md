# HTMLGtkFactory

HTMLGtkFactory creates GTK Widgets from HTML/DHMTL  layout!

### Basic Usage

``` 
#include <gtk/gtk.h>
#include <HTMLGtkFactory.h>

int main(int argc, char* argv) {

	HTMLGtkDocument* doc = htmlgtk_document_new_from_file( "myinterface.html" );

	GtkWidget* wnd = GTK_WIDGET(gtk_window_new(GTK_WINDOW_TOPLEVEL));
	gtk_container_add( GTK_CONTAINER(wnd), htmlgtk_document_get_body(doc) );

	gtk_widget_show_all(GTK_WIDGET(wnd));
	gtk_main();

	return 0;
}
``` 
