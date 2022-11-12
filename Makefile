
CC=gcc
CFLAGS=-Wall
BIN=langtons-ant

all: $(BIN)

$(BIN):	src/*.c src/*.h
	$(CC) $(CLFAGS) -o $@ src/main.c `sdl2-config --cflags --libs`

clean:
	rm -f $(BIN)
