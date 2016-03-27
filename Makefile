CC=gcc
DBG=cgdb
LIBS=-lm -lSDL
CFLAGS=-Wall -D_GNU_SOURCE=1 -D_REENTRANT -std=c99 -I/usr/include/SDL -g

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
BIN=demons

all: $(BIN)

run: $(BIN)
	./$(BIN)

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

debug: $(BIN)
	$(DBG) ./$(BIN)

clean:
	rm $(OBJ) $(BIN)
