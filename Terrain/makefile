# default target
all: product

# target: dependencies
# <TAB>command to build target from dependencies
product: terrain.o graphics.o glider.o
	g++ -std=c++11 -o product graphics.o terrain.o glider.o -lGL -lGLU -lglut

terrain.o: terrain.cpp terrain.h
	g++ -std=c++11 -c terrain.cpp

glider.o: glider.cpp glider.h
	g++ -std=c++11 -c glider.cpp

graphics.o: graphics.cpp graphics.h terrain.h glider.h
	g++ -std=c++11 -c graphics.cpp

run: product
	./product

clean:
	-rm -f product *.o