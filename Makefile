# Notes :
# 	-> $@ : current target
# 	-> $^ : dependency list
# 	-> $< : first dependency

CC=gcc
LD=gcc
CFLAGS=-g -Wall -I lib/include -Llib -Wl,-rpath,"$$"ORIGIN/../lib -pthread -lcheck
LDFLAGS=-lm
BIN=bin
SRC=src
HEADER=header
TEST=test

avalam: $(SRC)/main.o $(SRC)/plateau.o $(SRC)/ia.o
	$(LD) $(LDFLAGS) -o $(BIN)/$@ $^ $(CFLAGS)

test: $(TEST)/test.o $(SRC)/plateau.o $(SRC)/ia.o
	$(LD) $(LDFLAGS) -o $(TEST)/$@ $^ $(CFLAGS)

main.o: $(SRC)/main.c $(HEADER)/plateau.h
	$(CC) -c $< $(SRC)/plateau.c $(CFLAGS)
plateau.o: $(SRC)/plateau.c $(HEADER)/plateau.h
	$(CC) -c $< $(CFLAGS)
test.o : $(TEST)/test.c $(HEADER)/plateau.h
	$(CC) -c $< $(CFLAGS)
ia.o: $(SRC)/ia.c $(HEADER)/ia.h $(HEADER)/plateau.h
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f $(SRC)/*.o $(TEST)/*.o
