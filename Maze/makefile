# default target
all: product

# target: dependencies
# <TAB>command to build target from dependencies
product: maze.o graphics.o
	g++ -std=c++11 -o product graphics.o maze.o -lGL -lGLU -lglut

maze.o: maze.cpp maze.h
	g++ -std=c++11 -c maze.cpp

graphics.o: graphics.cpp graphics.h maze.h
	g++ -std=c++11 -c graphics.cpp

run: product
	./product

clean:
	-rm -f product *.o