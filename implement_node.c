
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
	return body_element->orig_widget;
}

GtkWidget*
HTMLGtkFactory_newline(factory_element_t* el) {
	GtkWidget* nl = GTK_WIDGET(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5));

	g_return_val_if_fail( GTK_IS_BOX(el->orig_widget), NULL );

	gtk_box_pack_start(GTK_BOX(el->orig_widget), nl, FALSE, FALSE, 0);

	el->widget = nl; // setup newline on element

	return nl;
}

GNode*
tree_add_node(GNode* parent, GNode* node) {
	g_node_append(parent,node);
}

GNode* this_parent;

gboolean
find_gumbo(GNode *node, gpointer data) {

	factory_element_t* search_el = (factory_element_t*)node->data;
	GumboNode* gumbo_node = (GumboNode*)data;
	if( search_el->gumbo_node == gumbo_node ) {
		this_parent = node; // FIXME: not use external variable.
		return TRUE;
	}
	else
		return FALSE;
}

GNode*
tree_find_by_gumbo(GumboNode* node) {

	this_parent = NULL;

	g_node_traverse( g_node_get_root(WidgetTree), G_IN_ORDER, G_TRAVERSE_ALL,
			 -1, find_gumbo, node );

	return this_parent;
}

factory_element_t*
find_parent_container(GNode* node) {

	GNode* p;
	factory_element_t* el;

	g_return_val_if_fail( node != NULL, NULL );

	p = node->parent;
	while( p ) {
		el = (factory_element_t*)p->data;
		if( GTK_IS_CONTAINER(el->widget) )
			return el;
		p = p->parent;
	}

	return NULL;

}

GtkWidget*
glue_node(GNode* node) {

	factory_element_t* el_container = find_parent_container(node);
	factory_element_t* el;

	g_return_val_if_fail( el_container != NULL , NULL );

	el = (factory_element_t*)node->data;

	if( GTK_IS_BOX(el->parent_widget) ) {
		gtk_box_pack_start(GTK_BOX(el->parent_widget), el->widget, FALSE, FALSE, 0);
		gtk_box_set_child_packing( GTK_BOX(el->parent_widget), el->widget,
					   FALSE, FALSE, 5, GTK_PACK_START );
	} else
		gtk_container_add(GTK_CONTAINER(el->parent_widget), el->widget);

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
					el->orig_widget = doc_body;
					body_element = el;
					HTMLGtkFactory_newline(body_element);
					break;
				case GUMBO_TAG_BR:
					gtk_widget_set_size_request( HTMLGtkFactory_newline(body_element), 
						1, 12 );
					return;
				case GUMBO_TAG_P:		
					el->orig_widget = el_p(el);
					HTMLGtkFactory_newline(/*el*/body_element);
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

	if( gumbo_node->type == GUMBO_NODE_ELEMENT )
		el->type = gumbo_node->v.element.tag;
	else
		el->type = 0xFFFF;

	parent = tree_find_by_gumbo(gumbo_node->parent);
	this_node = g_node_new((gpointer)el);

	if( (parent != NULL) && (el->widget != doc_body) ) {
		tree_add_node( parent, this_node);
		el->parent_widget = ((factory_element_t*)parent->data)->widget;
	} else
		tree_add_node( WidgetTree, this_node);

	glue_node( this_node );
}

