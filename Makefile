# Notes :
# 	-> $@ : current target
# 	-> $^ : dependency list
# 	-> $< : first dependency

CC=gcc
LD=gcc
X11_INCLUDES=-I /usr/include/X11/
X11_LINK=-L /usr/lib64/X11/

CFLAGS=-g -Wall -I lib/include -Llib $(X11_INCLUDES) $(X11_LINK) -Wl,-rpath,"$$"ORIGIN/../lib -pthread -lcheck -lX11 -lgraphics

LDFLAGS=-lm
BIN=bin
SRC=src
INCLUDE=include
#HEADER=header
TEST=test

avalam: $(SRC)/main.o $(SRC)/plateau.o $(SRC)/ia.o
	$(LD) $(LDFLAGS) -o $(BIN)/$@ $^ $(CFLAGS)

test: $(TEST)/test.o $(SRC)/plateau.o $(SRC)/ia.o
	$(LD) $(LDFLAGS) -o $(TEST)/$@ $^ $(CFLAGS)

main.o: $(SRC)/main.c $(INCLUDE)/plateau.h $(INCLUDE)/graphics.h
	$(CC) -c $< $(SRC)/plateau.c $(CFLAGS)
plateau.o: $(SRC)/plateau.c $(INCLUDE)/plateau.h
	$(CC) -c $< $(CFLAGS)
test.o : $(TEST)/test.c $(INCLUDE)/plateau.h
	$(CC) -c $< $(CFLAGS)
ia.o: $(SRC)/ia.c $(INCLUDE)/ia.h $(INCLUDE)/plateau.h
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f $(SRC)/*.o $(TEST)/*.o
