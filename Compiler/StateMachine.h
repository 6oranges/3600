#pragma once
#include"Token.h"
enum MachineState {
	START_STATE, IDENTIFIER_STATE, INTEGER_STATE, CANTMOVE_STATE,
	LPAREN_STATE,RPAREN_STATE,LCURLY_STATE,RCURLY_STATE,SEMICOLON_STATE,ASSIGNMENT_STATE,
	PLUS_STATE,MINUS_STATE,DIVIDE_STATE,TIMES_STATE,
	INCREMENT_STATE,DECREMENT_STATE,POWER_STATE,
	COMMENT_STATE,COMMENTSTAR_STATE,FULLCOMMENT_STATE,
	LINECOMMENT_STATE,
	PLUSEQUAL_STATE,MINUSEQUAL_STATE,TIMESEQUAL_STATE,DIVIDEEQUAL_STATE,
	LESS_STATE,GREATER_STATE,LESSEQUAL_STATE,GREATEREQUAL_STATE,
	EQUAL_STATE,NOTEQUAL_STATE,NOT_STATE,COMMA_STATE,
	INSERTION_STATE,EXTRACTION_STATE,
	STRING_STATE,STRINGSLASH_STATE,
	CHAR_STATE,CHARSLASH_STATE,HASCHAR_STATE,
	FULLCHAR_STATE,FULLSTRING_STATE,
	LAST_STATE
};

enum CharacterType {
	LETTER_CHAR, DIGIT_CHAR, WHITESPACE_CHAR,
	PLUS_CHAR, DIVIDE_CHAR, STAR_CHAR, MINUS_CHAR,
	SINGLEQUOTE_CHAR, DOUBLEQUOTE_CHAR,
	BAD_CHAR,NOT_CHAR,SLASH_CHAR,
	LPAREN_CHAR,RPAREN_CHAR,LCURLY_CHAR,RCURLY_CHAR,SEMICOLON_CHAR,EQUAL_CHAR,
	LESS_CHAR,GREATER_CHAR,NEWLINE_CHAR,EOF_CHAR,COMMA_CHAR,
	LAST_CHAR
};
class StateMachineClass
{
public:
	StateMachineClass();
	MachineState UpdateState(char currentCharacter, TokenType&
		correspondingTokenType);

private:
	MachineState mCurrentState;

	// The matrix of legal moves:
	MachineState mLegalMoves[LAST_STATE][LAST_CHAR];

	// Which end-machine-states correspond to which token types.
	// (non end states correspond to the BAD_TOKEN token type)
	TokenType mCorrespondingTokenTypes[LAST_STATE];
};