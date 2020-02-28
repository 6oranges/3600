#include <iostream>
#include "Parser.h"
#include "debug.h"
ParserClass::ParserClass(ScannerClass* sc, SymbolTableClass* stc)
: mScanner(sc), mSymbolTable(stc)
{}
TokenClass ParserClass::Match(TokenType expectedType)
{
	TokenClass currentToken = mScanner->GetNextToken();
	if(currentToken.GetTokenType() != expectedType)
	{
		std::cerr << "Error in ParserClass::Match. " << std::endl;
		std::cerr << "Expected token type " << 
			TokenClass:: GetTokenTypeName(expectedType) << 
            ", but got type " << currentToken.GetTokenTypeName() << std::endl;
		exit(1);
	}
	MSG("\tSuccessfully matched Token Type: " << 
		currentToken. GetTokenTypeName() << ". Lexeme: \"" << 
		currentToken.GetLexeme() << "\"");
	return currentToken; // the one we just processed
}
void ParserClass::Start(){
    Program();
	Match(ENDFILE_TOKEN);
}
void ParserClass::Program(){
	Match(VOID_TOKEN);
    Match(MAIN_TOKEN);
    Match(LPAREN_TOKEN);
    Match(RPAREN_TOKEN);
    Block();
}
void ParserClass::Block(){
	Match(LCURLY_TOKEN);
    StatementGroup();
    Match(RCURLY_TOKEN);
}
void ParserClass::StatementGroup(){
    bool going=true;
	while (going){
        going=Statement();
    }
}
bool ParserClass::Statement(){
    TokenType tt = mScanner->PeekNextToken().GetTokenType();
    switch (tt){
        case INT_TOKEN:{
            Match(tt);
            DeclarationStatement();
            break;
        }
        case IDENTIFIER_TOKEN:{
            Match(tt);
            AssignmentStatement();
            break;
        }
        case COUT_TOKEN:{
            Match(tt);
            CoutStatement();
            break;
        }
        default:{
            return false;
        }
    }
    return true;
}
void ParserClass::DeclarationStatement(){
    Match(INT_TOKEN);
    Match(IDENTIFIER_TOKEN);
    Match(SEMICOLON_TOKEN);
}
void ParserClass::AssignmentStatement(){
    Match(IDENTIFIER_TOKEN);
    Match(ASSIGNMENT_TOKEN);
    Expression();
}
void ParserClass::CoutStatement(){
    Match(COUT_TOKEN);
    Match(INSERTION_TOKEN);
    Expression();
}
void ParserClass::Expression(){
    Relational();
}
void ParserClass::Relational()
{
	PlusMinus();

	// Handle the optional tail:
	TokenType tt = mScanner->PeekNextToken().GetTokenType();
	if(tt == LESS_TOKEN)
	{
		Match(tt);
		PlusMinus();
	}
	else if(tt == LESSEQUAL_TOKEN)
	{
		Match(tt);
		PlusMinus();
	}
	else if(tt == GREATER_TOKEN)
	{
		Match(tt);
		PlusMinus();
	}
	else if(tt == GREATEREQUAL_TOKEN)
	{
		Match(tt);
		PlusMinus();
	}
	else if(tt == EQUAL_TOKEN)
	{
		Match(tt);
		PlusMinus();
	}
	else if(tt == NOTEQUAL_TOKEN)
	{
		Match(tt);
		PlusMinus();
	}
	else
	{
		return;
	}
}
void ParserClass::PlusMinus()
{
	TimesDivide();
	while(true)
	{
		TokenType tt = mScanner->PeekNextToken().GetTokenType();
		if(tt == PLUS_TOKEN)
		{
			Match(tt);
			TimesDivide();
		}
		else if(tt == MINUS_TOKEN)
		{
			Match(tt);
			TimesDivide();
		}
		else
		{
			return;
		}
	}
}
void ParserClass::TimesDivide()
{
	Factor();
	while(true)
	{
		TokenType tt = mScanner->PeekNextToken().GetTokenType();
		if(tt == TIMES_TOKEN)
		{
			Match(tt);
			Factor();
		}
		else if(tt == DIVIDE_TOKEN)
		{
			Match(tt);
			Factor();
		}
		else
		{
			return;
		}
	}
}
void ParserClass::Factor(){
	// Handle the optional tail:
	TokenType tt = mScanner->PeekNextToken().GetTokenType();
	if(tt == IDENTIFIER_TOKEN)
	{
		Match(tt);
	}
	else if(tt == INTEGER_TOKEN)
	{
		Match(tt);
	}
	else if(tt == LPAREN_TOKEN)
	{
		Match(tt);
		Expression();
        Match(RPAREN_TOKEN);
	}
	else
	{
		std::cerr<<"Expected a Factor but got something else"<<std::endl;
        exit(1);
	}
}