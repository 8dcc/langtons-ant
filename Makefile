
CC=gcc
CFLAGS=-Wall -Wextra
BIN=langtons-ant

.PHONY: all clean

all: $(BIN)

clean:
	rm -f $(BIN)

$(BIN):	src/main.c
	$(CC) $(CFLAGS) -o $@ $< `sdl2-config --cflags --libs`

