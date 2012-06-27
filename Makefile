
.PHONY: all clean
CC = g++
OPENGLFLAGS = -lGL -lGLU -lglut
SDLFLAGS = `sdl-config --cflags --libs`
objects = galaxy.o main.o star.o
all: $(objects)
	$(CC) $(CFLAGS) $(objects) $(OPENGLFLAGS) $(SDLFLAGS) -o simulation
galaxy.o: galaxy.cpp galaxy.h star.o
	$(CC) -c $(CFLAGS) galaxy.cpp
main.o: main.cpp galaxy.o
	$(CC) -c $(CFLAGS) main.cpp
star.o: star.cpp star.h
	$(CC) -c $(CFLAGS) star.cpp

clean: 
	rm *.o