#ifndef	_HTML_GTK_FACTORY_
#define	_HTML_GTK_FACTORY_

#include <gumbo.h>
#include <glib.h>
#include <gtk/gtk.h>
#include "element.h"

typedef struct _HTMLGtkDocument {
	factory_element_t*	body;
//	GtkCSSProvider*		style;
} HTMLGtkDocument;

typedef struct _HTMLGtkFactory {
	// document list
	//TODO: duk context;
} HTMLGtkFactory;

//HTMLGtkDocument*
//HTMLGtkFactory_load_file(const char* html_file);

GtkWidget*
HTMLGtkFactory_get_body(/* HTMLGtkDocument* doc*/);

factory_element_t*
HTMLGtkFactory_get_el_body();

GtkWidget*
HTMLGtkFactory_newline(factory_element_t*);

/* create GTK implementation of HTML node */
void
process_node(GumboNode*, int back);

void
tree_init(); // TODO: remove this

#endif
