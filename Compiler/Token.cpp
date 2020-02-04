
#include "Token.h"
#include "debug.h"
TokenClass::TokenClass() 
	:mType(BAD_TOKEN),mLexeme()
{

}
TokenClass::TokenClass(TokenType type, const std::string& lexeme) 
	:mType(type),mLexeme(lexeme)
{

}

void TokenClass::CheckReserved() {
	if (mType == IDENTIFIER_TOKEN) {
		if (mLexeme == "void") {
			mType = VOID_TOKEN;
		}
		if (mLexeme == "main") {
			mType = MAIN_TOKEN;
		}
		if (mLexeme == "int") {
			mType = INT_TOKEN;
		}
		if (mLexeme == "cout") {
			mType = COUT_TOKEN;
		}
	}
	else {
		//std::cout << "CheckReserved: Called with a token of " << GetTokenTypeName() << std::endl;
	}
}

std::ostream& operator<<(std::ostream& out, const TokenClass& tc) {
	out << " " << tc.GetTokenTypeName() << " '" << tc.GetLexeme() << "'";
	return out;
}