# HTMLGtkFactory

HTMLGtkFactory creates GTK Widgets from 'Web-like' HTML/DHMTL layout! Its *will* suport HTML, CSS and Javascript.

### Its userful for

* Easy and Quickly software interface design.
* Software scripting with interface.

### Basic Usage

##### The C Code
```C
#include <gtk/gtk.h>
#include <HTMLGtkFactory.h>

int main(int argc, char* argv) {

	HTMLGtkDocument* doc = htmlgtk_document_new_from_file( "test/form.html" );

	GtkWidget* wnd = GTK_WIDGET(gtk_window_new(GTK_WINDOW_TOPLEVEL));
	gtk_container_add( GTK_CONTAINER(wnd), htmlgtk_document_get_body(doc) );

	gtk_widget_show_all(GTK_WIDGET(wnd));
	gtk_main();

	return 0;
}
``` 

##### The HTML Layout
```html
<html>
<body>
	<h3>Login</h3>
	<br>
	<table>
		<tr>
			<td>Username:</td>
			<td><input name="uname"/></td>
		</tr>
		<tr>
			<td>Password:</td>
			<td><input name="passwd"/></td>
		</tr>
		<tr>
			<td></td>
			<td><button>Enter</button></td>
		</tr>
	</table>	
</body>
</html>
```

##### The Output

![](test/form.png?raw=true "Output for form.html")

### Comming Soon

* Javascript suport.
* Callback system in Javascript.
* Register custom C functions to Javascript context.
