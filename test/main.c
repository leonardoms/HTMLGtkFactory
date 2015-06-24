

#include <gumbo.h>
#include <stdio.h>
#include <HTMLGtkFactory.h>

int
main(int argc, char* argv[]) {	

	gtk_init(&argc, &argv);

	if( argc != 2 ) {
		g_print("usage:\n\t%s [html file]\n", argv[0]);
		return 1;
	}

	HTMLGtkDocument* doc = htmlgtk_document_new_from_file( argv[1] );

	g_return_val_if_fail( doc != NULL, 1 );

	GtkWidget* wnd = GTK_WIDGET(gtk_window_new(GTK_WINDOW_TOPLEVEL));
	GtkWidget* scroll = GTK_WIDGET(gtk_scrolled_window_new(NULL,NULL));

	gtk_container_add( GTK_CONTAINER(wnd), scroll );
	gtk_container_add( GTK_CONTAINER(scroll), htmlgtk_document_get_body(doc) );

	gtk_widget_show_all(GTK_WIDGET(wnd));

	gtk_main();

	return 0;
}
