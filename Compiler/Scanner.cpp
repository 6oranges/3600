#include "Scanner.h"
#include "StateMachine.h"
#include <sstream>
ScannerClass::ScannerClass(std::string filename)
{
	mFin.open(filename, std::ifstream::in);
	mLineNumber = 1;
	mColumn = 0;
	if (!mFin.good()) {
		std::cout << "ScannerClass: Something is unwell with your file" << std::endl;
		exit(1);
	}
}
ScannerClass::~ScannerClass() {
	mFin.close();
}
int ScannerClass::GetColumn() {
	return mColumn;
}
int ScannerClass::GetLineNumber() {
	return mLineNumber;
}
TokenClass ScannerClass::GetNextToken() {
	StateMachineClass stateMachine;
	TokenType tt;
	MachineState ms=START_STATE;
	std::stringstream lexeme;
	char c=-1;
	do {
		if (ms != START_STATE) {
			lexeme << c;
		}
		else {
			lexeme.str("");
		}
		if (c == '\n') {
			mLineNumber += 1;
			mColumn = 0;
		}
		c = mFin.get();
		mColumn += 1;
		ms = stateMachine.UpdateState(c, tt);
	} while (ms!=CANTMOVE_STATE);
	mColumn -= 1;
	if (!mFin&&tt==BAD_TOKEN) {
		tt = ENDFILE_TOKEN;
	}
	mFin.unget();
	if (tt==BAD_TOKEN){
		lexeme << c;
		std::cout << "ScannerClass::GetNextToken: lexeme: '" << lexeme.str() << "' at line " << mLineNumber << " and column " << mColumn << " resulted in a bad token" << std::endl;
		exit(1);
	}
	else {
		TokenClass tc = TokenClass(tt, lexeme.str());
		tc.CheckReserved();
		return tc;
	}
}