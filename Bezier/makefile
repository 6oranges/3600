# default target
all: product

# target: dependencies
# <TAB>command to build target from dependencies
product: main.o slider.o
	g++ -std=c++11 -o product main.o slider.o -lGL -lGLU -lglut

main.o: main.cpp slider.h
	g++ -std=c++11 -c main.cpp

slider.o: slider.cpp slider.h
	g++ -std=c++11 -c slider.cpp

run: product
	./product

clean:
	-rm -f product *.o