
#include <HTMLGtkFactory.h>

duk_ret_t htmlgtk_dom_document_getElementTest(duk_context* js_context) {
  // stack
  guint doc_addr;
  g_print("htmlgtk_dom_document_getElementTest doc_addr=");

  duk_push_this(js_context); // object

  duk_push_current_function(js_context);
  duk_get_prop_string(js_context, 0, "addr");
  doc_addr = duk_require_uint(js_context, -1);

  g_print("%d\n", doc_addr);
  return 1;
}

typedef struct {
  gchar* id;
  htmlgtk_element_t* el;
} search_element_id_t;

gboolean
find_id(GNode *node, gpointer data) {

	htmlgtk_element_t* cur_el = (htmlgtk_element_t*)node->data;
	search_element_id_t* srch_el_id = (search_element_id_t*)data;

  gpointer el_id = g_object_get_data(G_OBJECT(cur_el->orig_widget), "htmlgtk_id");

	if( !g_strcmp0(el_id,srch_el_id->id) ) {
		srch_el_id->el = cur_el;
		return TRUE;
	}
	else
		return FALSE;
}

duk_ret_t htmlgtk_dom_document_getElementById(duk_context* js_context) {
  guint doc_addr;
  HTMLGtkDocument* doc;
  search_element_id_t s_id;

  //g_print("getElementById -> %s\n", duk_get_string(js_context, -1));

  s_id.id = duk_get_string(js_context, -1);
  s_id.el = NULL;
  duk_pop(js_context);

  duk_push_this(js_context);
  duk_push_current_function(js_context);
  duk_get_prop_string(js_context, 0, "addr");

  doc_addr = duk_get_uint(js_context, -1);
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

void
htmlgtk_dom_document_create(HTMLGtkDocument* doc) {

	duk_push_object(doc->js_context);

	duk_push_uint(doc->js_context, (guint)doc);
	duk_put_prop_string(doc->js_context, -2, "addr");
	duk_push_c_function(doc->js_context, htmlgtk_dom_document_getElementById, 1);
	duk_put_prop_string(doc->js_context, -2, "getElementById");
  duk_push_c_function(doc->js_context, htmlgtk_dom_document_getElementTest, 0);
  duk_put_prop_string(doc->js_context, -2, "getElementTest");

	duk_put_global_string(doc->js_context, "document");
}
