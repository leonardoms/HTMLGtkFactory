

#include <gumbo.h>
#include <stdio.h>
//#include "HTMLGtkFactory.h"

void	implement_node(GumboNode* node) {

	switch( node->type ) {
		// Do specific things for type
		case GUMBO_NODE_ELEMENT: // TAGs here!
			// process TAG
			printf(" %s\t(childs=%2d)\tTHIS_%X PARENT_%X\n", gumbo_normalized_tagname(node->v.element.tag), node->v.element.children.length, node, node->parent);
			break;
		case GUMBO_NODE_TEXT:
			// process TEXT
			printf(" \ttext('%s')\tTHIS_%X PARENT_%X\n", node->v.text.text, node, node->parent);
			break;
		default:
			break;
	}
}

static int finished = 0;

static void	process_node(GumboNode* node, int back) {

	if( node != NULL )
	if( node->type != GUMBO_NODE_DOCUMENT ) {
		if( !back ) 
			implement_node(node); // process node!

		if( (node->type == GUMBO_NODE_TEXT) && !back ) {
			process_node(node->parent, 1); // child is not a element (is Text)
		} else
		if( (node->v.element.children.length > 0) && !back ) { 	// have Childs?
			process_node(node->v.element.children.data[0], 0); // process first Child
		} else
		if( node->parent->v.element.children.length > node->index_within_parent+1   ) { // Parent have more Childs?
			process_node(node->parent->v.element.children.data[node->index_within_parent+1], 0);
		}
		else
			process_node(node->parent, 1);
		return;

	} else return;
}

int
main(int argc, char* argv[]) {	
	GumboOutput* output = gumbo_parse(
		"<html><head><title></title></head><body><ul><li>Apples</li><li>Pears</li></ul><h3>TESTE</h3><div><select><select/><input><div>AAA</div></div><script></script></body></html>");

	process_node(output->root, 0);

	gumbo_destroy_output(&kGumboDefaultOptions, output);

	return 0;
}
