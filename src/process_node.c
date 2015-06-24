#include <HTMLGtkFactory.h>

void	process_node(GumboNode* node, int back) {

	if( node != NULL )
	if( node->type != GUMBO_NODE_DOCUMENT ) {
		if( !back ) 
			element_create(node); // process node!

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
