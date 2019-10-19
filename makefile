#changer le nom des deux lignes suivantes si nécessaire
SOURCE = Scrabble.c
OUTPUT = resultat
LIB = -lm -lSDL -lSDL_ttf
OPT = -Wall


SDL_CFLAGS = $(shell sdl-config --cflags)
SDL_LDFLAGS = $(shell sdl-config --libs)


all: $(OUTPUT)

clean: $(OUTPUT)
	rm $(OUTPUT) *.o lib/*.o

$(OUTPUT): lib/libgraphique.o source.o
	@gcc lib/libgraphique.o source.o -o $(OUTPUT)  $(SDL_CFLAGS) $(SDL_LDFLAGS) $(LIB)


lib/libgraphique.o: lib/libgraphique.c
	@gcc   -c lib/libgraphique.c -o lib/libgraphique.o 

source.o: $(SOURCE)
	@gcc $(OPT) -c $(SOURCE)  -o source.o






