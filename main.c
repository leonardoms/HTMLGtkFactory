

#include <gumbo.h>
#include <stdio.h>
#include "HTMLGtkFactory.h"

static void	process_node(GumboNode* node, int back) {

	if( node != NULL )
	if( node->type != GUMBO_NODE_DOCUMENT ) {
		if( !back ) 
			implement_node(node); // process node!

		if( node->type != GUMBO_NODE_TEXT && node->type != GUMBO_NODE_WHITESPACE &&
		    node->type != GUMBO_NODE_CDATA && node->type != GUMBO_NODE_COMMENT &&
		    node->type != GUMBO_NODE_TEMPLATE ) {
			if( (node->v.element.children.length > 0) && !back ) { 	// have Childs?
				process_node(node->v.element.children.data[0], 0); // process first Child
				return;
			}
		}

		if( node->parent->v.element.children.length > node->index_within_parent+1 ) { // Parent have more Childs?
			process_node(node->parent->v.element.children.data[node->index_within_parent+1], 0);
		}
		else
			process_node(node->parent, 1);

		return;

	} else
	return;
}

int
main(int argc, char* argv[]) {	

	gtk_init(&argc, &argv);

	GumboOutput* output = gumbo_parse(
		"<html>"
		"<head>"
		"	<link rel=\"stylesheet\" href=\"test/default.css\">"
		"</head>"
		"<body>"
		"<h1>Main Heading Topic</h1>"
		"<h2>This is a Subtopic</h2>"
		"<br/>"
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
		"</table>"
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
