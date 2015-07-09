
#include <HTMLGtkFactory.h>

duk_ret_t htmlgtk_dom_element_getter(duk_context* js_context) {
  // stack
  // -1 -> rcv
  // -2 -> key
  // -3 -> target
  // TODO: check args
  guint el_addr;
  htmlgtk_element_t* el;
  gpointer  el_data;

  gchar* key = duk_require_string(js_context, -2);

  duk_push_this(js_context); // object
  duk_push_current_function(js_context);
  duk_get_prop_string(js_context, 0, "addr");

  el_addr = duk_require_uint(js_context, -1);
  el = (htmlgtk_element_t*)el_addr;

  g_return_val_if_fail(GTK_IS_WIDGET(el->orig_widget), NULL);

 el_data = g_object_get_data(G_OBJECT(el->orig_widget), g_strdup_printf("htmlgtk_%s", key));
 if( el_data != NULL ) {
    duk_push_string(js_context, g_strdup_printf("%s", el_data));
  }
  else {
    duk_push_null(js_context);
  }

  return 1;
}

duk_ret_t htmlgtk_dom_element_setter(duk_context* js_context) {
  // stack
  // -1 -> rcv
  // -2 -> value
  // -3 -> key
  // -4 -> target
  const gchar *key, *value;
  key = g_ascii_strdown(duk_require_string(js_context,-3),-1);
  value = duk_require_string(js_context,-2);

  if(!g_strcmp0("innerhtml", key)) {

  } else
    g_warning("unknown or invalid property `%s`.", key);

  g_print("htmlgtk_dom_element_setter -> [%s %s]\n", duk_require_string(js_context,-2), duk_require_string(js_context,-3));


  return 0;
}

duk_ret_t my_c_getter_2(duk_context *ctx) {
	g_print("C_GETTER for property: %s\n", duk_get_string(ctx, -2));
	duk_push_uint(ctx, 54321);
	return 1;
}

duk_ret_t my_c_setter_2(duk_context *ctx) {
	g_print("C_SETTER\n");// for property: %s\n", duk_get_string(ctx, -2));

	return 0;
}

void
htmlgtk_dom_element_create(HTMLGtkDocument* doc, htmlgtk_element_t* el) {

  duk_eval_string(doc->js_context, "htmlgtk_js_object"); // function to create new object (see 'core.js')
	duk_push_uint(doc->js_context, (guint)el);
	duk_push_c_function(doc->js_context, htmlgtk_dom_element_getter, 3);
	duk_push_c_function(doc->js_context, htmlgtk_dom_element_setter, 4);
	duk_call(doc->js_context,3);

}
