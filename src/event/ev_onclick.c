
#include <HTMLGtkFactory.h>

void
ev_onclick(GtkButton *sender, gpointer user_data) {
	
	gpointer doc_ptr, evt_data;

	doc_ptr = g_object_get_data( G_OBJECT(sender), "htmlgtkdocument" );
	g_return_if_fail(doc_ptr != NULL);

	evt_data = g_object_get_data(G_OBJECT(sender), "htmlgtk_onclick" );

	htmlgtk_script_run_string( (HTMLGtkDocument*)doc_ptr, g_strdup_printf("%s", evt_data) ); 
}
