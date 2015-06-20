
#include "HTMLGtkFactory.h"
#include <glib.h>

GNode *WidgetTree;
static GtkWidget* doc_body, *body_row;
factory_element_t* body_element; // user for glue widgets on corret row!

void
tree_init() {
	WidgetTree = g_node_new(g_malloc0(sizeof(factory_element_t)));
	doc_body = NULL;
	body_row = NULL;
	body_element = NULL;
}

GtkWidget*
HTMLGtkFactory_get_body() {
	return doc_body;
}

GtkWidget*
HTMLGtkFactory_newline() {
	GtkWidget* nl = GTK_WIDGET(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5));

	body_element->widget = nl; // setup newline on body element

	gtk_container_add(GTK_CONTAINER(doc_body), nl);
}

GNode*
tree_add_node(GNode* parent, GNode* node) {
	g_node_append(parent,node);
}

GNode* this_parent;

gboolean
find_gumbo(GNode *node, gpointer data) {
	g_print("find_gumbo");
	factory_element_t* search_el = (factory_element_t*)node->data;
	GumboNode* gumbo_node = (GumboNode*)data;
	if( search_el->gumbo_node == gumbo_node ) {
		this_parent = node; // FIXME
		return TRUE;
	}
	else
		return FALSE;
}

GNode*
tree_find_by_gumbo(GumboNode* node) {
	g_print("tree_find_by_gumbo");
	this_parent = NULL;

	g_node_traverse( g_node_get_root(WidgetTree), G_IN_ORDER, G_TRAVERSE_ALL,
			 -1, find_gumbo, node );

	return this_parent;
}

GtkWidget*
find_parent_container(GNode* node) {
	g_print("find_parent_container");
	GNode* p;
	factory_element_t* el;

	g_return_val_if_fail( node != NULL, NULL );

	p = node->parent;
	while( p ) {
		el = (factory_element_t*)p->data;
		if( GTK_IS_CONTAINER(el->widget) )
			return el->widget;
		p = p->parent;
	}

	return NULL;

}

GtkWidget*
glue_node(GNode* node) {
	g_print("glue_node");
	GtkWidget* container = find_parent_container(node);
	factory_element_t* el;

	g_return_val_if_fail( GTK_IS_CONTAINER(container), NULL );

	el = (factory_element_t*)node->data;
	gtk_container_add(GTK_CONTAINER(container), el->widget);

	if( GTK_IS_BOX(container) ) {
		gtk_box_set_child_packing( GTK_BOX(container), el->widget,
					   FALSE, FALSE, 5, GTK_PACK_START );
	}

	return el->widget;
}

void
implement_node(GumboNode* gumbo_node) {

	factory_element_t* el = g_malloc0(sizeof(factory_element_t));
	GNode *parent, *this_node;

	el->gumbo_node = gumbo_node;

	switch( gumbo_node->type ) {
		case GUMBO_NODE_ELEMENT:
			switch(gumbo_node->v.element.tag) {
				case GUMBO_TAG_BODY:
					doc_body = el_body(el);
					body_element = el;
				case GUMBO_TAG_P:
				case GUMBO_TAG_BR: // TODO: BR tag must be add a dummy widget with default vertical size.				
					HTMLGtkFactory_newline();
					break;
				case GUMBO_TAG_BUTTON:
					el_button(el);
					break;
				default:
					g_warning("unsuported or invalid tag (%s)", gumbo_normalized_tagname(gumbo_node->v.element.tag));
					return;
			}
			break;
		case GUMBO_NODE_TEXT:
			el_text(el);
			break;
		default:
			break;
	}

	parent = tree_find_by_gumbo(gumbo_node->parent);
	this_node = g_node_new((gpointer)el);

	if( (parent != NULL) && (el->widget != doc_body) ) {
		tree_add_node( parent, this_node);
	} else
		tree_add_node( WidgetTree, this_node);

	glue_node( this_node );
}

