#include <iostream>
#include <sstream>
#include "Parser.h"
#include "Node.h"
#include "Debug.h"

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
            ", but got " << currentToken << std::endl;
		exit(1);
	}
	MSG("\tSuccessfully matched Token Type: " << 
		currentToken. GetTokenTypeName() << ". Lexeme: \"" << 
		currentToken.GetLexeme() << "\"");
	return currentToken; // the one we just processed
}
StartNode * ParserClass::Start()
{
	ProgramNode *pn = Program();
	Match(ENDFILE_TOKEN);
	StartNode * sn = new StartNode(pn);
	return sn;
}

ProgramNode * ParserClass::Program(){
	Match(VOID_TOKEN);
    Match(MAIN_TOKEN);
    Match(LPAREN_TOKEN);
    Match(RPAREN_TOKEN);
    BlockNode* b = Block();
	return new ProgramNode(b);
}
BlockNode* ParserClass::Block(){
	Match(LCURLY_TOKEN);
    StatementGroupNode* s = StatementGroup();
    Match(RCURLY_TOKEN);
	return new BlockNode(s);
}
StatementGroupNode* ParserClass::StatementGroup(){
    StatementNode* s=Statement();
	StatementGroupNode* ss = new StatementGroupNode();
	while (s!=NULL){
		ss->AddStatement(s);
        s=Statement();
    }
	return ss;
}
StatementNode* ParserClass::Statement(){
    TokenType tt = mScanner->PeekNextToken().GetTokenType();
    switch (tt){
        case INT_TOKEN:{
            return DeclarationStatement();
            break;
        }
        case IDENTIFIER_TOKEN:{
            return AssignmentStatement();
            break;
        }
        case COUT_TOKEN:{
            return CoutStatement();
            break;
        }
		case IF_TOKEN:{
            return IfStatement();
            break;
        }
		case WHILE_TOKEN:{
            return WhileStatement();
            break;
        }
        case LCURLY_TOKEN:{
            return Block();
            break;
        }
        default:{
            return NULL;
        }
    }
}
IdentifierNode* ParserClass::Identifier(){
	TokenClass tc = Match(IDENTIFIER_TOKEN);
	IdentifierNode* in = new IdentifierNode(tc.GetLexeme(),mSymbolTable);
    return in;
}
IntegerNode* ParserClass::Integer(){
	return new IntegerNode(atoi(Match(INTEGER_TOKEN).GetLexeme().c_str()));
}
DeclarationStatementNode* ParserClass::DeclarationStatement(){
    Match(INT_TOKEN);
	IdentifierNode* in = Identifier();
    Match(SEMICOLON_TOKEN);
	return new DeclarationStatementNode(in);
}
IfStatementNode* ParserClass::IfStatement(){
    Match(IF_TOKEN);
	Match(LPAREN_TOKEN);
	ExpressionNode* en = Expression();
	Match(RPAREN_TOKEN);
	StatementNode* sn = Statement();
	IfStatementNode* in = new IfStatementNode(sn,en);
	TokenType tt = mScanner->PeekNextToken().GetTokenType();
	if (tt==ELSE_TOKEN){
		Match(tt);
		StatementNode* e = Statement();
		in->AddElse(e);
	}
	return in;
}
WhileNode* ParserClass::WhileStatement(){
    Match(WHILE_TOKEN);
	Match(LPAREN_TOKEN);
	ExpressionNode* en = Expression();
	Match(RPAREN_TOKEN);
	StatementNode* sn = Statement();
	WhileNode* wn = new WhileNode(sn,en);
	return wn;
}
AssignmentStatementNode* ParserClass::AssignmentStatement(){
    IdentifierNode* in = Identifier();

	TokenType tt = mScanner->PeekNextToken().GetTokenType();
	switch (tt){
		case ASSIGNMENT_TOKEN:{
			Match(ASSIGNMENT_TOKEN);
			ExpressionNode* en = Expression();
			Match(SEMICOLON_TOKEN);
			return new AssignmentStatementNode(in,en);
		}
		case PLUSEQUAL_TOKEN:{
			Match(PLUSEQUAL_TOKEN);
			ExpressionNode* en = Expression();
			Match(SEMICOLON_TOKEN);
			return new PlusEqualNode(in,en);
		}
		case MINUSEQUAL_TOKEN:{
			Match(MINUSEQUAL_TOKEN);
			ExpressionNode* en = Expression();
			Match(SEMICOLON_TOKEN);
			return new MinusEqualNode(in,en);
		}
	}
    
}
CoutStatementNode* ParserClass::CoutStatement(){
    Match(COUT_TOKEN);
    Match(INSERTION_TOKEN);
    ExpressionNode* en = ExpressionOrNull();
	CoutStatementNode* c = new CoutStatementNode();
	c->addExpression(en);
	while (true){
		TokenType tt = mScanner->PeekNextToken().GetTokenType();
		if (tt==INSERTION_TOKEN){
			Match(tt);
			ExpressionNode* en2 = ExpressionOrNull();
			c->addExpression(en2);
		}
		else if (tt==SEMICOLON_TOKEN){
			Match(SEMICOLON_TOKEN);
			return c;
		}
	}
}
ExpressionNode* ParserClass::ExpressionOrNull(){
	TokenType tt = mScanner->PeekNextToken().GetTokenType();
	if (tt==ENDL_TOKEN){
		Match(tt);
		return NULL;
	}
	else{
		return Expression();
	}
}
ExpressionNode* ParserClass::Expression(){
    return Or();
}
ExpressionNode* ParserClass::Or()
{
	ExpressionNode * current = And();
	while(true)
	{
		TokenType tt = mScanner->PeekNextToken().GetTokenType();
		if(tt == OR_TOKEN)
		{
			Match(tt);
			current = new OrNode(current, And());
		}
		else
		{
			return current;
		}
	}
}
ExpressionNode* ParserClass::And()
{
	ExpressionNode * current = Relational();
	while(true)
	{
		TokenType tt = mScanner->PeekNextToken().GetTokenType();
		if(tt == AND_TOKEN)
		{
			Match(tt);
			current = new AndNode(current, Relational());
		}
		else
		{
			return current;
		}
	}
}
ExpressionNode* ParserClass::Relational()
{
	ExpressionNode* current = PlusMinus();

	// Handle the optional tail:
	TokenType tt = mScanner->PeekNextToken().GetTokenType();
	if(tt == LESS_TOKEN)
	{
		Match(tt);
		return new LessNode(current,PlusMinus());
	}
	else if(tt == LESSEQUAL_TOKEN)
	{
		Match(tt);
		return new LessEqualNode(current,PlusMinus());
	}
	else if(tt == GREATER_TOKEN)
	{
		Match(tt);
		return new GreaterNode(current,PlusMinus());
	}
	else if(tt == GREATEREQUAL_TOKEN)
	{
		Match(tt);
		return new GreaterEqualNode(current,PlusMinus());
	}
	else if(tt == EQUAL_TOKEN)
	{
		Match(tt);
		return new EqualNode(current,PlusMinus());
	}
	else if(tt == NOTEQUAL_TOKEN)
	{
		Match(tt);
		return new NotEqualNode(current,PlusMinus());
	}
	else
	{
		return current;
	}
}
ExpressionNode* ParserClass::PlusMinus()
{
	ExpressionNode * current = TimesDivide();
	while(true)
	{
		TokenType tt = mScanner->PeekNextToken().GetTokenType();
		if(tt == PLUS_TOKEN)
		{
			Match(tt);
			current = new PlusNode(current, TimesDivide());
		}
		else if(tt == MINUS_TOKEN)
		{
			Match(tt);
			current = new MinusNode(current, TimesDivide());
		}
		else
		{
			return current;
		}
	}
}
ExpressionNode* ParserClass::TimesDivide()
{
	ExpressionNode * current = Factor();
	while(true)
	{
		TokenType tt = mScanner->PeekNextToken().GetTokenType();
		if(tt == TIMES_TOKEN)
		{
			Match(tt);
			current = new TimesNode(current, Factor());
		}
		else if(tt == DIVIDE_TOKEN)
		{
			Match(tt);
			current = new DivideNode(current, Factor());
		}
		else
		{
			return current;
		}
	}
}
ExpressionNode* ParserClass::Factor(){
	// Handle the optional tail:
	TokenType tt = mScanner->PeekNextToken().GetTokenType();
	if(tt == IDENTIFIER_TOKEN)
	{
		return Identifier();
	}
	else if(tt == INTEGER_TOKEN)
	{
		return Integer();
	}
	else if(tt == LPAREN_TOKEN)
	{
		Match(tt);
		ExpressionNode* e= Expression();
        Match(RPAREN_TOKEN);
		return e;
	}
	else
	{
		std::cerr<<"Expected a Factor but got something else"<<std::endl;
        exit(1);
	}
}
