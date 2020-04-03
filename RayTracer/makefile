# default target
all: product

# target: dependencies
# <TAB>command to build target from dependencies
product: main.o vector.o
	g++ -std=c++11 -o product main.o vector.o -lGL -lGLU -lglut

main.o: main.cpp vector.h graphics.h
	g++ -std=c++11 -c main.cpp

vector.o: vector.cpp vector.h
	g++ -std=c++11 -c vector.cpp

run: product
	./product

clean:
	-rm -f product *.o