# default target
all: product

# target: dependencies
# <TAB>command to build target from dependencies
product: Main.o Scanner.o Token.o StateMachine.o Symbol.o
	g++ -std=c++11 -o product Main.o Scanner.o Token.o StateMachine.o Symbol.o

Scanner.o: Scanner.cpp Scanner.h StateMachine.h Debug.h
	g++ -std=c++11 -c Scanner.cpp

Token.o: Token.cpp Token.cpp Debug.h
	g++ -std=c++11 -c Token.cpp

StateMachine.o: StateMachine.cpp StateMachine.h Token.h Debug.h
	g++ -std=c++11 -c StateMachine.cpp

Main.o: Main.cpp Debug.h
	g++ -std=c++11 -c Main.cpp

Symbol.o: Symbol.cpp Symbol.h
	g++ -std=c++11 -c Symbol.cpp

run: product
	./product

clean:
	-rm -f product *.o