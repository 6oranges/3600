
#include "Token.h"
#include "Debug.h"
TokenClass::TokenClass() 
	:mType(BAD_TOKEN),mLexeme()
{

}
TokenClass::TokenClass(TokenType type, const std::string& lexeme) 
	:mType(type),mLexeme(lexeme)
{
	MSG("RECIEVING:"<<TokenClass::GetTokenTypeName(type));

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
		if (mLexeme == "if") {
			mType = IF_TOKEN;
		}
		if (mLexeme == "else") {
			mType = ELSE_TOKEN;
		}
		if (mLexeme == "while") {
			mType = WHILE_TOKEN;
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
