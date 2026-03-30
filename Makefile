CC = gcc
CFLAGS = $(shell sdl2-config --cflags) -Wall
LIBS = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf -lSDL2_mixer

all: mon_jeu

mon_jeu: src/main.o src/scores.o src/menu.o src/jeu.o
	$(CC) -o mon_jeu src/main.o src/scores.o src/menu.o src/jeu.o $(LIBS)

src/main.o: src/main.c src/scores.h src/menu.h src/jeu.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/scores.o: src/scores.c src/scores.h
	$(CC) $(CFLAGS) -c src/scores.c -o src/scores.o

src/menu.o: src/menu.c src/menu.h
	$(CC) $(CFLAGS) -c src/menu.c -o src/menu.o

src/jeu.o: src/jeu.c src/jeu.h
	$(CC) $(CFLAGS) -c src/jeu.c -o src/jeu.o

clean:
	rm -f src/*.o mon_jeu
