#ifndef	_HTML_GTK_FACTORY_
#define	_HTML_GTK_FACTORY_

#include <gumbo.h>
#include <glib.h>
#include <gtk/gtk.h>

GtkWidget*
HTMLGtkFactory_get_body();

typedef	struct factory_element {
	GumboNode*	gumbo_node;
	GtkWidget*	widget;
} factory_element_t;

/* create GTK implementation of HTML node */
void
implement_node(GumboNode*);

/* find the near Parent GtkContainer */
GtkWidget*
find_parent_container(GNode*);

/* place Widget */
GtkWidget*
glue_node(GNode* node);

void
tree_init();

#endif
