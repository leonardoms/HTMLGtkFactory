

#include <gumbo.h>
#include <stdio.h>
#include "HTMLGtkFactory.h"

static void	process_node(GumboNode* node, int back) {

	if( node != NULL )
	if( node->type != GUMBO_NODE_DOCUMENT ) {
		if( !back ) 
			implement_node(node); // process node!

//		if( (node->type == GUMBO_NODE_TEXT) && !back ) {
//			//FIXME: Se tem irmao, processar ele!
//			process_node(node->parent, 1); // child is not a element (is Text)
//		} else
		if( node->type != GUMBO_NODE_TEXT ) {
			if( (node->v.element.children.length > 0) && !back ) { 	// have Childs?
				process_node(node->v.element.children.data[0], 0); // process first Child
			}
		}

		if( node->parent->v.element.children.length > node->index_within_parent+1 && !back  ) { // Parent have more Childs?
			process_node(node->parent->v.element.children.data[node->index_within_parent+1], 0);
		}
		else
			process_node(node->parent, 1);

		return;

	} else return;
}

int
main(int argc, char* argv[]) {	

	gtk_init(&argc, &argv);

	GumboOutput* output = gumbo_parse(
		"<html><head></head><body>Lorem Ipsum...<button>MyButton</button>HTMLGtkFactory<br/>Second line: <button>Button 2!</button><br/><br/><p>Using basic <p>HTML for</p></p><p>GTK 3.0 Programming!</p></body></html>");

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
