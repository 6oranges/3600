#pragma once
#include <string>
#include <iostream>
enum TokenType {
	// Reserved Words:
	VOID_TOKEN, MAIN_TOKEN, INT_TOKEN, COUT_TOKEN,
	// Relational Operators:
	LESS_TOKEN, LESSEQUAL_TOKEN, GREATER_TOKEN, GREATEREQUAL_TOKEN, EQUAL_TOKEN, NOTEQUAL_TOKEN,
	// Other Operators:
	INSERTION_TOKEN, EXTRACTION_TOKEN, ASSIGNMENT_TOKEN,
	PLUS_TOKEN, MINUS_TOKEN, TIMES_TOKEN, DIVIDE_TOKEN,
	PLUSEQUAL_TOKEN, MINUSEQUAL_TOKEN, TIMESEQUAL_TOKEN, DIVIDEEQUAL_TOKEN,
	INCREMENT_TOKEN,DECREMENT_TOKEN,POWER_TOKEN,NOT_TOKEN,
	AND_TOKEN,OR_TOKEN,
	BITAND_TOKEN,BITOR_TOKEN,
	// Other Characters:
	SEMICOLON_TOKEN, LPAREN_TOKEN, RPAREN_TOKEN, LCURLY_TOKEN,
	RCURLY_TOKEN,COMMA_TOKEN,
	// Other Token Types:
	IDENTIFIER_TOKEN, INTEGER_TOKEN,COMMENT_TOKEN,
	STRING_TOKEN,CHAR_TOKEN,
	IF_TOKEN,ELSE_TOKEN,WHILE_TOKEN,
	BAD_TOKEN, ENDFILE_TOKEN
};
const std::string gTokenTypeNames[] = {
	"VOID", "MAIN", "INT", "COUT",
"LESS", "LESSEQUAL", "GREATER", "GREATEREQUAL", "EQUAL", "NOTEQUAL",
	"INSERTION", "EXTRACTION","ASSIGNMENT",
	"PLUS", "MINUS", "TIMES", "DIVIDE",
	"PLUSEQUAL", "MINUSEQUAL", "TIMESEQUAL", "DIVIDEEQUAL",
	"INCREMENT","DECREMENT","POWER","NOT",
	"AND","OR",
	"BITAND","BITOR",
	"SEMICOLON", "LPAREN", "RPAREN", "LCURLY", "RCURLY","COMMA_TOKEN",
	"IDENTIFIER", "INTEGER","COMMENT",
	"STRING","CHAR",
	"IF","ELSE","WHILE",
	"BAD", "ENDFILE"
};
class TokenClass
{
private:
	TokenType mType;
	std::string mLexeme;
public:
	TokenClass();
	TokenClass(TokenType type, const std::string& lexeme);
	TokenType GetTokenType() const { return mType; }
	const std::string& GetTokenTypeName() const
	{
		return gTokenTypeNames[mType];
	}
	std::string GetLexeme() const { return mLexeme; }
	void CheckReserved();
	static const std::string& GetTokenTypeName(TokenType type)
	{
		return gTokenTypeNames[type];
	}
};
std::ostream& operator<<(std::ostream& out, const TokenClass& tc);
