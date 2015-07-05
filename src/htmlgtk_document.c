#include <HTMLGtkFactory.h>

duk_ret_t my_c_getter(duk_context *ctx) {
	g_print("C_GETTER for property: %s\n", duk_get_string(ctx, -2));
	duk_push_uint(ctx, 54321);
	return 1;
}

duk_ret_t my_c_setter(duk_context *ctx) {
	g_print("C_SETTER\n");// for property: %s\n", duk_get_string(ctx, -2));

	return 0;
}

HTMLGtkDocument*
htmlgtk_document_new() {

	HTMLGtkDocument* doc = g_malloc0(sizeof(HTMLGtkDocument));

	doc->tree = NULL;

	doc->js_context = duk_create_heap_default();
	g_return_val_if_fail( doc->js_context != NULL, NULL );

	doc->tree = g_node_new(g_malloc0(sizeof(htmlgtk_element_t)));

	GBytes* res_mastercss = g_resources_lookup_data("/htmlgtk/master.css", G_RESOURCE_LOOKUP_FLAGS_NONE, NULL);
	gsize	sz;

	if(res_mastercss != NULL) {
		GtkCssProvider *css = gtk_css_provider_get_default();
		GdkDisplay *display = gdk_display_get_default ();
		GdkScreen *screen = gdk_display_get_default_screen (display);
		gpointer css_data = (gpointer)g_bytes_get_data(res_mastercss, &sz);

		gtk_style_context_add_provider_for_screen (screen,
					GTK_STYLE_PROVIDER (css),
					GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

		gtk_css_provider_load_from_data(GTK_CSS_PROVIDER(css),css_data, sz, NULL);
	} else {
		g_warning("htmlgtk_document_new() fails loading default style.");
	}

	GBytes* res_corejs = g_resources_lookup_data("/htmlgtk/htmlgtk_core.js", G_RESOURCE_LOOKUP_FLAGS_NONE, NULL);
	g_return_val_if_fail( res_corejs != NULL, NULL);

	gpointer js_data = (gpointer)g_bytes_get_data(res_corejs, &sz);

//	g_print("eval-> %s", js_data);

	duk_eval_string(doc->js_context, js_data);

	//test
/*	duk_eval_string(doc->js_context, "htmlgtk_js_object"); // function to create new object (see 'core.js')
	duk_push_uint(doc->js_context, (guint)doc); // for example //FIXME: change to a JS object
	duk_push_c_lightfunc(doc->js_context, my_c_getter, 3, 3, 0);
	duk_push_c_lightfunc(doc->js_context, my_c_setter, 4, 4, 0);
	duk_call(doc->js_context,3); // run-> htmlgtk_js_object(doc,my_c_getter,my_c_setter);

	// save the new object
	duk_put_global_string(doc->js_context, "myobj"); // myobj = htmlgtk_js_object(doc,my_c_getter,my_c_setter);

	duk_eval_string(doc->js_context, "myobj.innerHTML = 'test';");
	duk_eval_string(doc->js_context, "print(myobj.obj_addr);");
*/
//  test
//	duk_eval_string(doc->js_context, "htmlgtk_js_test");
//	duk_push_string(doc->js_context, "foo");
//	duk_call(doc->js_context, 1);

	htmlgtk_dom_document_create(doc);

	return doc;
}

HTMLGtkDocument*
htmlgtk_document_new_from_string(const gchar* str) {

	HTMLGtkDocument* doc;

	g_return_val_if_fail( str != NULL, NULL );

	doc = htmlgtk_document_new();
	g_return_val_if_fail( doc != NULL, NULL );

	GumboOutput* output = gumbo_parse( str ); // create element tree.

	htmlgtk_document_process_node(doc, output->root, 0); // process all nodes

	gumbo_destroy_output(&kGumboDefaultOptions, output);

	return doc;
}

HTMLGtkDocument*
htmlgtk_document_new_from_file(const gchar* file) {

	gchar* str;
	gsize length;

	g_file_get_contents( file, &str, &length, NULL ); // TODO: error checking.

	g_return_val_if_fail( length != 0, NULL );

	return htmlgtk_document_new_from_string(str);
};

GtkWidget*
htmlgtk_document_get_body(HTMLGtkDocument* doc) {
	g_return_val_if_fail( doc != NULL, NULL );
	g_return_val_if_fail( doc->body != NULL, NULL );

	return doc->body->orig_widget;
}

GtkWidget*
htmlgtk_document_newline(HTMLGtkDocument* doc, htmlgtk_element_t* el) {
	GtkWidget* nl = GTK_WIDGET(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5));

	g_return_val_if_fail( GTK_IS_BOX(el->orig_widget), NULL );

	gtk_box_pack_start(GTK_BOX(el->orig_widget), nl, FALSE, FALSE, 0);

	el->widget = nl; // setup newline on element

	return nl;
}


void	htmlgtk_document_process_node(HTMLGtkDocument* doc, GumboNode* node, int back) {

	g_return_if_fail( doc != NULL );

	if( node != NULL )
	if( node->type != GUMBO_NODE_DOCUMENT ) {
		if( !back )
			htmlgtk_document_element_create(doc, node); // process node!

		if( node->type != GUMBO_NODE_TEXT && node->type != GUMBO_NODE_WHITESPACE &&
		    node->type != GUMBO_NODE_CDATA && node->type != GUMBO_NODE_COMMENT &&
		    node->type != GUMBO_NODE_TEMPLATE ) {
			if( (node->v.element.children.length > 0) && !back ) { 	// have Childs?
				htmlgtk_document_process_node(doc, node->v.element.children.data[0], 0); // process first Child
				return;
			}
		}

		if( node->parent->v.element.children.length > node->index_within_parent+1 ) { // Parent have more Childs?
			htmlgtk_document_process_node(doc, node->parent->v.element.children.data[node->index_within_parent+1], 0);
		}
		else
			htmlgtk_document_process_node(doc, node->parent, 1);

		return;

	} else
	return;
}

///////////////////////////////////
// ELEMENT CREATE FUNCTIONS
//////////////////////////////////

GtkStyleContext* style_ctx;
GNode* this_parent;

GNode*
tree_add_node(HTMLGtkDocument* doc, GNode* parent, GNode* node) {
	g_node_append(parent,node);
}


gboolean
find_gumbo(GNode *node, gpointer data) {

	htmlgtk_element_t* search_el = (htmlgtk_element_t*)node->data;
	GumboNode* gumbo_node = (GumboNode*)data;
	if( search_el->gumbo_node == gumbo_node ) {
		this_parent = node; // FIXME: not use external variable.
		return TRUE;
	}
	else
		return FALSE;
}

gboolean
find_gtk(GNode *node, gpointer data) {

	htmlgtk_element_t* search_el = (htmlgtk_element_t*)node->data;
	GtkWidget* w = (GtkWidget*)data;
	if( search_el->orig_widget == w || search_el->widget == w ) {
		this_parent = node; // FIXME: not use external variable.
		return TRUE;
	}
	else
		return FALSE;
}

GNode*
tree_find_by_gumbo(HTMLGtkDocument* doc, GumboNode* node) {

	this_parent = NULL;

	g_node_traverse( g_node_get_root(doc->tree), G_IN_ORDER, G_TRAVERSE_ALL,
			 -1, find_gumbo, node );

	return this_parent;
}

GNode*
tree_find_by_gtk(HTMLGtkDocument* doc, GtkWidget* w) {

	this_parent = NULL;

	g_node_traverse( g_node_get_root(doc->tree), G_IN_ORDER, G_TRAVERSE_ALL,
			 -1, find_gtk, w );

	return this_parent;
}

htmlgtk_element_t*
find_parent_container(HTMLGtkDocument* doc, GNode* node) {

	GNode* p;
	htmlgtk_element_t* el;

	g_return_val_if_fail( node != NULL, NULL );

	p = node->parent;
	while( p ) {
		el = (htmlgtk_element_t*)p->data;
		if( GTK_IS_CONTAINER(el->widget) )
			return el;
		p = p->parent;
	}

	return NULL;

}

htmlgtk_element_t*
find_parent_container_by_gumbo(HTMLGtkDocument* doc, GumboNode* node) {

	GNode* p;
	htmlgtk_element_t* el;

	g_return_val_if_fail( node != NULL, NULL );

	p = tree_find_by_gumbo(doc, node);

	g_return_val_if_fail( p != NULL, NULL );

	//p = p->parent;
	while( p ) {
		el = (htmlgtk_element_t*)p->data;
		if( GTK_IS_CONTAINER(el->widget) )
			return el;
		p = p->parent;
	}

	return NULL;

}

htmlgtk_element_t*
find_parent_table(HTMLGtkDocument* doc, GumboNode* node) {

	GNode* p;
	htmlgtk_element_t* el;
	GumboNode*	gn;

	g_return_val_if_fail( node != NULL, NULL );

	gn = node->parent;

	g_return_val_if_fail( gn != NULL, NULL );

	while( gn->type == GUMBO_NODE_ELEMENT ) {
		if( gn->v.element.tag == GUMBO_TAG_TABLE ) {
			p = tree_find_by_gumbo(doc, gn);
			g_return_val_if_fail(p != NULL, NULL);
			el = (htmlgtk_element_t*)p->data;
			return el;
		}
		gn = gn->parent;
	}

	return NULL;

}

void
htmlgtk_document_event_create(HTMLGtkDocument* doc, htmlgtk_element_t* el) {

	if( g_object_get_data(G_OBJECT(el->orig_widget), "htmlgtk_onclick" ) != NULL )
		g_signal_connect( G_OBJECT(el->orig_widget), "clicked", G_CALLBACK(ev_onclick), el ); //FIXME: 'clicked' only works with buttons


}

GtkWidget*
glue_node(HTMLGtkDocument* doc, GNode* node) {

	htmlgtk_element_t* el_container = find_parent_container(doc, node);
	htmlgtk_element_t* el;

	g_return_val_if_fail( el_container != NULL || GTK_IS_WIDGET(el->widget) == FALSE, NULL );

	el = (htmlgtk_element_t*)node->data;

	if( GTK_IS_BOX(el->parent_widget) ) {
		gtk_box_pack_start(GTK_BOX(el->parent_widget), el->widget, FALSE, FALSE, 0);
		gtk_box_set_child_packing( GTK_BOX(el->parent_widget), el->widget,
					   FALSE, FALSE, 5, GTK_PACK_START );
	} else
	if( GTK_IS_GRID(el->parent_widget) ) {
		gpointer data;
		data = g_object_get_data( G_OBJECT(el->widget), "pos" );
		g_return_if_fail( data != NULL );

		gtk_grid_attach( GTK_GRID(el->parent_widget), el->widget, ((guint*)data)[1], ((guint*)data)[0],
				((guint*)data)[2], ((guint*)data)[3] );
	} else
		gtk_container_add(GTK_CONTAINER(el->parent_widget), el->widget);

	return el->widget;
}

//TODO: newline relative to structural master parent container. eg.: body, div, frame..
void
htmlgtk_document_element_create(HTMLGtkDocument* doc, GumboNode* gumbo_node) {
	// g_print("implement_node -> %s\n", gumbo_normalized_tagname(gumbo_node->v.element.tag));
	htmlgtk_element_t* el = g_malloc0(sizeof(htmlgtk_element_t));
	htmlgtk_element_t* el_tmp;
	GNode *parent, *this_node;
	GumboAttribute* att;
	gpointer	data, data2;
	GtkWidget*	widget;

	el->gumbo_node = gumbo_node;
	parent = tree_find_by_gumbo(doc, gumbo_node->parent);

	switch( gumbo_node->type ) {
		case GUMBO_NODE_ELEMENT:
			switch(gumbo_node->v.element.tag) {
				case GUMBO_TAG_TABLE:
					el_table(el);
					htmlgtk_document_newline(doc, doc->body);
					break;
				case GUMBO_TAG_TH:
				case GUMBO_TAG_TR:
					el_tmp = find_parent_table( doc, gumbo_node );

					g_return_if_fail( el_tmp != NULL );

					g_return_if_fail(GTK_IS_GRID( el_tmp->widget ) );

					data = g_object_get_data( G_OBJECT(el_tmp->widget), "last_pos" );
					g_return_if_fail( data != NULL );

					((guint*)data)[0]++; // row++
					((guint*)data)[1] = 0; // col = 0
					return;
				case GUMBO_TAG_TD:
					el_tmp = find_parent_table( doc, gumbo_node );

					g_return_if_fail( el_tmp != NULL );

					g_return_if_fail(GTK_IS_GRID( el_tmp->widget ) );

					data = g_object_get_data( G_OBJECT(el_tmp->widget), "last_pos" );
					g_return_if_fail( data != NULL );

					// save TD position
					data2 = g_malloc0( sizeof(guint)*4 );	// top, left, xspan, yspan
					((guint*)data2)[0] = ((guint*)data)[0];
					((guint*)data2)[1] = ((guint*)data)[1];

					el_p(el);

					att = gumbo_get_attribute(&gumbo_node->v.element.attributes, "colspan");
					if( att != NULL )
						sscanf(att->value, "%d", &(((guint*)data2)[2]) );
					if( ((guint*)data2)[2] < 1 )
						((guint*)data2)[2] = 1;

					att = gumbo_get_attribute(&gumbo_node->v.element.attributes, "rowspan");
					if( att != NULL )
						sscanf(att->value, "%d", &(((guint*)data2)[3]) );
					if( ((guint*)data2)[3] < 1 )
						((guint*)data2)[3] = 1;

					g_object_set_data( G_OBJECT(el->widget), "pos", data2);
					//htmlgtk_document_newline(el);
					((guint*)data)[1] += ((guint*)data2)[2]; // col++

					parent = tree_find_by_gumbo(doc, el_tmp->gumbo_node);
					break;
				case GUMBO_TAG_LINK:
					att = gumbo_get_attribute(&gumbo_node->v.element.attributes, "rel");
					if( att != NULL ) {
						if( !g_strcmp0( att->value,"stylesheet" ) ) {
							att = gumbo_get_attribute(&gumbo_node->v.element.attributes, "href");
							if( att != NULL ) {
								GtkCssProvider *css = gtk_css_provider_get_default();
								GdkDisplay *display = gdk_display_get_default ();
								GdkScreen *screen = gdk_display_get_default_screen (display);

								gtk_style_context_add_provider_for_screen (screen,
											GTK_STYLE_PROVIDER (css),
											GTK_STYLE_PROVIDER_PRIORITY_USER);

								gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(css), att->value, NULL);

							}
						}
					}
					return;
				case GUMBO_TAG_BODY:
					el_body(el);
					doc->body = el;
					htmlgtk_document_newline(doc, doc->body);
					break;
				case GUMBO_TAG_BR:
					gtk_widget_set_size_request( htmlgtk_document_newline(doc, doc->body),
						1, 24 );
					return;
				case GUMBO_TAG_BUTTON:
					el_button(el);
					break;
				case GUMBO_TAG_P:
				case GUMBO_TAG_H1:
				case GUMBO_TAG_H2:
				case GUMBO_TAG_H3:
				case GUMBO_TAG_H4:
				case GUMBO_TAG_H5:
					el_p(el);
					htmlgtk_document_newline(doc, doc->body);
					break;
				case GUMBO_TAG_A: //TODO: implement click
				case GUMBO_TAG_I:
				case GUMBO_TAG_B:
				case GUMBO_TAG_STRONG:
				case GUMBO_TAG_SMALL: //FIXME: jinho
					el_text_base(el);
					break;
				case GUMBO_TAG_INPUT:	//TODO: implement 'type' attribute
					el_input_text(el);
					break;
				case GUMBO_TAG_SCRIPT:
					return;	// do nothing here
				default:
					g_warning("unsuported or invalid tag (%s)", gumbo_normalized_tagname(gumbo_node->v.element.tag));
					return;
			}
			break;
		case GUMBO_NODE_TEXT:
			if( el->gumbo_node->parent->type == GUMBO_NODE_ELEMENT )
			if( el->gumbo_node->parent->v.element.tag == GUMBO_TAG_SCRIPT )
				htmlgtk_script_run_string( doc, el->gumbo_node->v.text.text ); // run script
			else
				el_text(el); // print text
			break;
		case GUMBO_NODE_WHITESPACE:
			// ignored whitespace
			return;
		default:
			g_warning("unsuported or invalid element type (%d)", gumbo_node->type);
			return;
	}

	g_object_set_data( G_OBJECT(el->orig_widget), "htmlgtkdocument", (gpointer)doc );

	if( gumbo_node->type == GUMBO_NODE_ELEMENT ) {
		el->type = gumbo_node->v.element.tag;
		htmlgtk_element_attributes_read(el);
//		att = gumbo_get_attribute(&gumbo_node->v.element.attributes, "name");
//		if( att != NULL ) {
//		if( att == NULL ) // if name is not set -> use tag as name
//			gtk_widget_set_name( GTK_WIDGET(el->widget), att->value ); // set name
//		} else
			gtk_widget_set_name( GTK_WIDGET(el->widget), gumbo_normalized_tagname(gumbo_node->v.element.tag) ); // set default name
	}
	else
		el->type = 0xFFFF;

	this_node = g_node_new((gpointer)el);

	if( (parent != NULL) && (el != doc->body) ) {
		tree_add_node( doc, parent, this_node);
		el->parent_widget = ((htmlgtk_element_t*)parent->data)->widget;
		glue_node( doc, this_node );
	} else
		tree_add_node( doc, doc->tree, this_node);

	htmlgtk_document_event_create( doc, el );

}
