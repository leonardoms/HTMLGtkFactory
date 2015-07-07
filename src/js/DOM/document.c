
#include <HTMLGtkFactory.h>

typedef struct {
  gchar* param;
  gint   param_i;
  htmlgtk_element_t* el;
  HTMLGtkDocument* doc;
  duk_idx_t idx;
} search_element_t;

gboolean
find_id(GNode *node, gpointer data) {

	htmlgtk_element_t* cur_el = (htmlgtk_element_t*)node->data;
	search_element_t* srch_el_id = (search_element_t*)data;

  gpointer el_id = g_object_get_data(G_OBJECT(cur_el->orig_widget), "htmlgtk_id");

	if( !g_strcmp0(el_id,srch_el_id->param) ) {
		srch_el_id->el = cur_el;
		return TRUE;
	}
	else
		return FALSE;
}

duk_ret_t htmlgtk_dom_document_getElementById(duk_context* js_context) {
  guint doc_addr;
  HTMLGtkDocument* doc;
  search_element_t s_id;

  //g_print("getElementById -> %s\n", duk_get_string(js_context, -1));

  s_id.param = duk_require_string(js_context, -1);
  s_id.el = NULL;
  duk_pop(js_context);

  duk_push_this(js_context);
  duk_push_current_function(js_context);
  duk_get_prop_string(js_context, 0, "addr");

  doc_addr = duk_require_uint(js_context, -1);
  doc = (HTMLGtkDocument*)doc_addr;

  g_return_val_if_fail(doc_addr != 0, 0);

  g_node_traverse( g_node_get_root(doc->tree), G_IN_ORDER, G_TRAVERSE_ALL,
			 -1, find_id, &s_id );

  if(s_id.el == NULL) {
    duk_push_null(js_context);
  } else {
    htmlgtk_dom_element_create(doc, s_id.el);
  }

  return 1;
}

gboolean
find_name(GNode *node, gpointer data) {
  htmlgtk_element_t* cur_el = (htmlgtk_element_t*)node->data;
	search_element_t* srch_el_name = (search_element_t*)data;
  gchar* cur_name;

  g_return_val_if_fail(GTK_IS_WIDGET(cur_el->orig_widget), FALSE);

  cur_name = gtk_widget_get_name(GTK_WIDGET(cur_el->orig_widget));

  //g_print("cur_name -> %s (search: %s)\n", cur_name, srch_el_name->param);

  if( g_strcmp0(cur_name, srch_el_name->param) )
    return FALSE; // not match

  htmlgtk_dom_element_create(srch_el_name->doc, cur_el);
  duk_put_prop_index(srch_el_name->doc->js_context, srch_el_name->idx, srch_el_name->param_i);
//  duk_pop(srch_el_name->doc->js_context);

  srch_el_name->param_i++;
  return FALSE;
}

duk_ret_t htmlgtk_dom_document_getElementsByTagName(duk_context* js_context) {
  guint doc_addr;
  HTMLGtkDocument* doc;
  search_element_t s_name;

  s_name.param = duk_require_string(js_context, -1);
  s_name.param_i = 0;
  duk_pop(js_context);

  duk_push_this(js_context);
  duk_push_current_function(js_context);
  duk_get_prop_string(js_context, 0, "addr");

  doc_addr = duk_require_uint(js_context, -1);
  doc = (HTMLGtkDocument*)doc_addr;

  g_return_val_if_fail(doc_addr != 0, 0);

  s_name.doc = doc;
  s_name.idx = duk_push_array(js_context);
  g_node_traverse( g_node_get_root(doc->tree), G_IN_ORDER, G_TRAVERSE_ALL,
       -1, find_name, &s_name );

  if(s_name.param_i == 0) {
      duk_push_null(js_context);
  }

  return 1;
}

void
htmlgtk_dom_document_create(HTMLGtkDocument* doc) {

	duk_push_object(doc->js_context);

	duk_push_uint(doc->js_context, (guint)doc);
	duk_put_prop_string(doc->js_context, -2, "addr"); //TODO: make read only (Proxy? duk_def_prop?)
	duk_push_c_function(doc->js_context, htmlgtk_dom_document_getElementById, 1);
	duk_put_prop_string(doc->js_context, -2, "getElementById");
  duk_push_c_function(doc->js_context, htmlgtk_dom_document_getElementsByTagName, 1);
	duk_put_prop_string(doc->js_context, -2, "getElementsByTagName");

	duk_put_global_string(doc->js_context, "document");

  duk_pop(doc->js_context);
}
