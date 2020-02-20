all:
	gcc -Wall -g -I/usr/include/X11 -L/usr/X11R6/lib polymin.c dlx.c\
		-o polymin -lXm -lXt -lX11 -lXpm -lXext -ljpeg -lXmu -lpthread
