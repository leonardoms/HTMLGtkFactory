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

int main(int argc, char* argv[]) {

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
			<td><button id="loginBtn" name="Foo" onClick="make_login();">Enter</button></td>
		</tr>
	</table>
	<script type="text/javascript">
		print('button name is -> ' + document.getElementById('loginBtn').name);

		var inputs = document.getElementsByTagName('input');
		for(i = 0; i < inputs.length; i++)
			print('input#' + i + ' name is ' + inputs[i].name);
	</script>
</body>
</html>
```

##### The Output

![](test/form.png?raw=true "Output for form.html")

and on terminal prints:

```
user@htmlgtk-pc~$ ./main test/form.html
button name is -> Foo
input#0 name is uname
input#1 name is passwd
user@htmlgtk-pc~$
```

### Dependencies

* [Gumbo Parser](https://github.com/google/gumbo-parser) library.

### Comming Soon

* Javascript suport with DOM.
* HTML element events (OnClick, OnHover, ...)
* Register custom C functions to Javascript context.
