#ifndef	_HTML_GTK_FACTORY_
#define	_HTML_GTK_FACTORY_

#include <gumbo.h>
#include <duktape.h>
#include <glib.h>
#include <gtk/gtk.h>

typedef	struct factory_element {
	unsigned	type;
	GumboNode*	gumbo_node; // g&l
	GtkWidget	*widget, *parent_widget, *orig_widget;
	void		(*post_create)(struct factory_element*);
} factory_element_t;

typedef struct _HTMLGtkDocument {
	factory_element_t*	body;
	GNode *tree;
	duk_context*		js_context;
//	GtkCSSProvider*		style;
} HTMLGtkDocument;

typedef struct _HTMLGtkFactory {
	// document list
} HTMLGtkFactory;

HTMLGtkDocument*
htmlgtk_document_new();

HTMLGtkDocument*
htmlgtk_document_new_from_string(const gchar* str);

HTMLGtkDocument*
htmlgtk_document_new_from_file(const gchar* file);

GtkWidget*
htmlgtk_document_get_body(HTMLGtkDocument* doc);

GtkWidget*
htmlgtk_document_newline(HTMLGtkDocument* doc, factory_element_t*);

/* create GTK implementation of HTML node */
void
htmlgtk_document_process_node(HTMLGtkDocument* doc, GumboNode*, int back);

///////////////////////////
// Element functions
//////////////////////////

// create html element on GTK interface.
void	
htmlgtk_document_element_create(HTMLGtkDocument* doc, GumboNode* gumbo_node);

// read all attributes from html element and store on gobject 'orig_widget'.
int	// return count of attributes.
htmlgtk_document_element_attributes_read(factory_element_t* el);

#endif
