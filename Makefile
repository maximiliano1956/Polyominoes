all:
	gcc -Wall -g -O2 -I/usr/include/X11 -L/usr/X11R6/lib polymin.c \
		-o polymin -lXm -lXt -lX11 -lXpm -lXext -ljpeg -lXmu
