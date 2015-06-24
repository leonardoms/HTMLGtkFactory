

#include <gumbo.h>
#include <stdio.h>
#include <HTMLGtkFactory.h>

int
main(int argc, char* argv[]) {	

	gtk_init(&argc, &argv);

	GumboOutput* output = gumbo_parse(
		"<html>"
		"<head>"
		"	<link rel=\"stylesheet\" href=\"resources/master.css\">"
		"</head>"
		"<body>"
		"<h1>The GTK Project</h1>"
		"<h2>What is GTK+, and how can I use it?</h2>"
		"<br/>"
		"GTK+, or the GIMP Toolkit, is a multi-platform toolkit for<br/>"
		"create <a>graphical user interfaces.</a><br/>"
		"AFDPA<p>Put your email here: <input></p>Table example:"
		"<table>"
		"	<tr>"
		"		<td></td><td>Col#1</td><td>Col#2</td><td>Col#3</td>"
		"	</tr>"
		"	<tr>"
		"		<td>Row#1</td><td colspan=\"2\" name=\"colspan_test\"><button>A1 + B1</button></td><td><button>C1</button></td>"
		"	</tr>"
		"	<tr>"
		"		<td>Row#2</td><td><button>A2</button></td><td><button>B2</button></td><td name=\"rowspan_test\" rowspan=\"2\"><button>C2</button></td>"
		"	</tr>"
		"	<tr>"
		"		<td>Row#2</td><td><button>A3</button></td><td><button>B3</button></td>" //<td><button>C3</button></td>"
		"	</tr>"
		"</table><br/>" // TODO: auto newline after table
		"<strong>Note:</strong> The progress tag is not supported<br/> in Internet Explorer 9 and earlier versions."
		"</body>"
		"</html>" );

	tree_init();
	process_node(output->root, 0);

	gumbo_destroy_output(&kGumboDefaultOptions, output);

	GtkWidget* wnd = GTK_WIDGET(gtk_window_new(GTK_WINDOW_TOPLEVEL));
	GtkWidget* scroll = GTK_WIDGET(gtk_scrolled_window_new(NULL,NULL));

	gtk_container_add( GTK_CONTAINER(wnd), scroll );
	gtk_container_add( GTK_CONTAINER(scroll), HTMLGtkFactory_get_body() );

	gtk_widget_show_all(GTK_WIDGET(wnd));

	gtk_main();

	return 0;
}
