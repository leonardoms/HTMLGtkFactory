#ifndef	_ELEMENT_H_
#define _ELEMENT_H_

#include <gumbo.h>
#include <glib.h>
#include <gtk/gtk.h>

typedef	struct factory_element {
	unsigned	type;
	GumboNode*	gumbo_node; // g&l
	GtkWidget	*widget, *parent_widget, *orig_widget;
} factory_element_t;

// create html element on GTK interface.
void	
element_create(GumboNode* gumbo_node);

// read all attributes from html element and store on gobject
int	// return count of attributes.
element_attributes_read(factory_element_t* el);

#endif
