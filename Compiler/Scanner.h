#pragma once
#include <fstream>
#include "Token.h"
class ScannerClass {
public:
	ScannerClass(std::string filename);
	~ScannerClass();
	TokenClass GetNextToken();
	TokenClass PeekNextToken();
	int GetLineNumber();
	int GetColumn();
private:
	std::ifstream mFin;
	int mLineNumber;
	int mColumn;
};
