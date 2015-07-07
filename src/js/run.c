
#include <HTMLGtkFactory.h>

void
htmlgtk_script_run_string(HTMLGtkDocument* doc, const gchar* cmd) {
	duk_push_string(doc->js_context, cmd);
	if (duk_peval(doc->js_context) != 0) {
	    printf("** ERROR script syntax: %s\n", duk_safe_to_string(doc->js_context, -1));
	}
	duk_pop(doc->js_context);
}

void
htmlgtk_script_run_file(HTMLGtkDocument* doc, const gchar* file) {
	if (duk_peval_file(doc->js_context, file) != 0) {
		g_warning("%s", duk_safe_to_string(doc->js_context, -1));
	}
}
