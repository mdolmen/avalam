
### Configuration

X11_INCLUDES=-I/usr/include/X11/
X11_LINK=-L/usr/lib64/X11/ -lX11
CC=gcc
CFLAGS=$(X11_INCLUDES) 
LIB=~/Documents/avalam/lib
INCLUDE=~/Documents/avalam/lib

#-O -fno-defer-pop -Wall
RANLIB=ranlib

##########################################################################

OBJS=open.o draw.o fill.o color.o text.o \
     image.o make_img.o dump_img.o point_col.o sound.o events.o

all: libgraphics.a

libgraphics.a: $(OBJS)
	rm -f libgraphics.a
	ar rc libgraphics.a $(OBJS)
	$(RANLIB) libgraphics.a

test: test.o libgraphics.a
	$(CC) -o test test.o -L. -lgraphics $(X11_LINK) -lm

install:
	cp libgraphics.a $(LIB)
	cp graphics.h $(INCLUDE)

clean:
	rm -f *~ *.o *.a
	rm -f test



