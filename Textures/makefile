# default target
all: product

# target: dependencies
# <TAB>command to build target from dependencies
product: graphics.o
	g++ -std=c++11 -o product graphics.o -lGL -lGLU -lglut

graphics.o: graphics.cpp
	g++ -std=c++11 -c graphics.cpp

run: product
	./product

clean:
	-rm -f product *.o