# This Makefile turns off exec protection
CXXFLAGS=-fno-stack-protector -D_FORTIFY_SOURCE=0
CXXFLAGS += -std=c++11 

toy: Main.o Token.o Symbol.o Parser.o Scanner.o Node.o Instructions.o StateMachine.o
	$(CXX) -o $@ $^ 
	execstack -s $@
instructions: Instructions.o Instructions_tester.o
	$(CXX) -o $@ $^ 
	execstack -s $@

Instructions_tester.o: Instructions.h
Instructions.o: Instructions.cpp Instructions.h
Symbol.o: Symbol.cpp Symbol.h Debug.h
Token.o: Token.cpp Token.h Debug.h
Scanner.o: Scanner.cpp Scanner.h Debug.h Token.h StateMachine.h
Node.o: Node.cpp Node.h Debug.h Symbol.h Instructions.h
Parser.o: Parser.cpp Parser.h Debug.h Token.h Scanner.h Symbol.h Node.h
Main.o: Main.cpp Scanner.h Symbol.h Parser.h Node.h Token.h Instructions.h Debug.h
StateMachine.o: StateMachine.cpp StateMachine.h Token.h Debug.h

clean:
	@echo "doing a clean:"
	rm -f *.o toy instructions
