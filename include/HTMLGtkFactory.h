#ifndef	_HTML_GTK_FACTORY_
#define	_HTML_GTK_FACTORY_

#include <gumbo.h>
#include <duktape.h>
#include <glib.h>
#include <gtk/gtk.h>

typedef	struct htmlgtk_element {
	GtkWidget	*orig_widget, *widget, *parent_widget;
	unsigned	type;
	GumboNode*	gumbo_node; // g&l
//	void		(*post_create)(struct factory_element*);
//	void		(*child_add)(factory_element_t*, factory_element_t*); // specific funtion to join child
} htmlgtk_element_t;

typedef struct _HTMLGtkDocument {
	htmlgtk_element_t*	body;
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
htmlgtk_document_newline(HTMLGtkDocument* doc, htmlgtk_element_t*);

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
htmlgtk_document_element_attributes_read(htmlgtk_element_t* el);

///////////////////////////
// Event functions
//////////////////////////

void
ev_onclick(GtkButton *sender, gpointer user_data);

void
htmlgtk_dom_document_create(HTMLGtkDocument* doc);

void
htmlgtk_dom_element_create(HTMLGtkDocument* doc, htmlgtk_element_t* el);

#endif
