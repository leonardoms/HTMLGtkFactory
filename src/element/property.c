
#include <HTMLGtkFactory.h>

int
htmlgtk_element_attributes_read(htmlgtk_element_t* el) {

	gpointer	prop;
	gpointer	name;
	GumboAttribute* att;
	int	i;

	g_return_val_if_fail(el->gumbo_node->type == GUMBO_NODE_ELEMENT, -1);

	if( el->gumbo_node->v.element.attributes.length > 0 ) {
		for( i = 0; i < el->gumbo_node->v.element.attributes.length; i++ ) {
			att = el->gumbo_node->v.element.attributes.data[i];
			prop = g_strdup_printf( "%s", att->value );
			name = g_strdup_printf( "htmlgtk_%s", att->name );
			g_object_set_data( G_OBJECT(el->orig_widget), name, prop ); // save property
		}
	}

	return el->gumbo_node->v.element.attributes.length;
}
