# default target
all: product

# target: dependencies
# <TAB>command to build target from dependencies
product: Main.o Scanner.o Token.o StateMachine.o Symbol.o Node.o Parser.o Instructions.o
	g++ -std=c++11 -o product Main.o Scanner.o Token.o StateMachine.o Symbol.o Node.o Parser.o Instructions.o

Scanner.o: Scanner.cpp Scanner.h StateMachine.h Debug.h
	g++ -std=c++11 -c Scanner.cpp

Instructions.o: Instructions.cpp Instructions.h Debug.h
	g++ -std=c++11 -c Instructions.cpp

Token.o: Token.cpp Token.cpp Debug.h
	g++ -std=c++11 -c Token.cpp

StateMachine.o: StateMachine.cpp StateMachine.h Token.h Debug.h
	g++ -std=c++11 -c StateMachine.cpp

Main.o: Main.cpp Parser.h Debug.h
	g++ -std=c++11 -c Main.cpp

Node.o: Node.cpp Node.h Symbol.h Debug.h
	g++ -std=c++11 -c Node.cpp

Parser.o: Parser.cpp Parser.h Symbol.h Scanner.h Debug.h
	g++ -std=c++11 -c Parser.cpp

Symbol.o: Symbol.cpp Symbol.h
	g++ -std=c++11 -c Symbol.cpp

run: product
	./product

clean:
	-rm -f product *.o