#ifndef	_HTML_GTK_FACTORY_
#define	_HTML_GTK_FACTORY_

#include <gumbo.h>
#include <glib.h>
#include <gtk/gtk.h>

typedef struct _HTMLGtkDocument {

} HTMLGtkDocument;

typedef struct _HTMLGtkFactory {
	// document list
} HTMLGtkFactory;

//HTMLGtkDocument*
//HTMLGtkFactory_load_file(const char* html_file);

GtkWidget*
HTMLGtkFactory_get_body(/* HTMLGtkDocument* doc*/);

typedef	struct factory_element {
	unsigned	type;
	GumboNode*	gumbo_node;
	GtkWidget	*widget, *parent_widget, *orig_widget;
} factory_element_t;

/* create GTK implementation of HTML node */
void
implement_node(GumboNode*);

void
tree_init();

#endif
