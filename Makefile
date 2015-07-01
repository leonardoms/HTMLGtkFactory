
C_SOURCE = 	src/event/ev_onclick.c	\
		src/htmlgtk_document.c	\
		src/htmlgtk_factory.c	\
		test/main.c		\
		src/element/el_text_base.c	\
		src/element/el_input.c	\
		src/element/el_text.c	\
		src/element/el_body.c	\
		src/element/el_p.c      \
		src/element/property.c	\
		src/element/el_button.c	\
		src/element/el_table.c	\
		src/element/el_td.c	\
		src/js/run.c		\
		src/js/third/duktape.c	\
		resources/resources.c

#RESOURCE_FILES = resources/master.css

all:
#	glib-compile-resources --target resources/resources.c --generate-source resources/resources.gresource.xml
	cc `pkg-config --libs --cflags gumbo gtk+-3.0` -lm -o main -g -Iinclude/ $(C_SOURCE)
