# default target
all: product

# target: dependencies
# <TAB>command to build target from dependencies
product: chessstarter.o
	g++ -std=c++11 -o product chessstarter.o -lGL -lGLU -lglut

chessstarter.o: chessstarter.cpp
	g++ -std=c++11 -c chessstarter.cpp

run: product
	./product

clean:
	-rm -f product *.o