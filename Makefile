
C_SOURCE = 	src/htmlgtk_document.c	\
		src/htmlgtk_factory.c	\
		src/main.c		\
		src/process_node.c	\
		src/element/el_a.c	\
		src/element/el_input.c	\
		src/element/el_text.c	\
		src/element/el_body.c	\
		src/element/el_p.c      \
		src/element/property.c	\
		src/element/el_button.c	\
		src/element/el_table.c	\
		src/element/element.c	\
		src/element/el_td.c

all:
	cc `pkg-config --libs --cflags gumbo gtk+-3.0` -o main -g -Iinclude/ $(C_SOURCE)
