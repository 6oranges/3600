# default target
all: product

# target: dependencies
# <TAB>command to build target from dependencies
product: chessstarter.o objloader.h slider.o vectors.h
	g++ -std=c++11 -o product chessstarter.o slider.o -lGL -lGLU -lglut

chessstarter.o: chessstarter.cpp
	g++ -std=c++11 -c chessstarter.cpp
	
slider.o: slider.cpp slider.h
	g++ -std=c++11 -c slider.cpp

run: product
	./product

clean:
	-rm -f product *.o