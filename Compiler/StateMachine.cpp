#include "StateMachine.h"
#include "Debug.h"
StateMachineClass::StateMachineClass() {
	mCurrentState = START_STATE;
	for (int i = 0; i < LAST_STATE; i++)
	{
		for (int j = 0; j < LAST_CHAR; j++)
		{
			mLegalMoves[i][j] = CANTMOVE_STATE;
		}
	}
	// Strings
	mLegalMoves[START_STATE][DOUBLEQUOTE_CHAR] = STRING_STATE;
	for (int i = 0;i < LAST_CHAR;i++) {
		mLegalMoves[STRING_STATE][i] = STRING_STATE;
	}
	mLegalMoves[STRING_STATE][DOUBLEQUOTE_CHAR] = FULLSTRING_STATE;
	mLegalMoves[STRING_STATE][SLASH_CHAR] = STRINGSLASH_STATE;
	for (int i = 0;i < LAST_CHAR;i++) {
		mLegalMoves[STRINGSLASH_STATE][i] = STRING_STATE;
	}
	// Chars
	mLegalMoves[START_STATE][SINGLEQUOTE_CHAR] = CHAR_STATE;
	for (int i = 0;i < LAST_CHAR;i++) {
		mLegalMoves[CHAR_STATE][i] = HASCHAR_STATE;
	}
	mLegalMoves[HASCHAR_STATE][SINGLEQUOTE_CHAR] = FULLCHAR_STATE;
	mLegalMoves[CHAR_STATE][SLASH_CHAR] = CHARSLASH_STATE;
	for (int i = 0;i < LAST_CHAR;i++) {
		mLegalMoves[CHARSLASH_STATE][i] = CHAR_STATE;
	}
	// Identifiers
	mLegalMoves[START_STATE][LETTER_CHAR]=IDENTIFIER_STATE;
	mLegalMoves[IDENTIFIER_STATE][LETTER_CHAR]= IDENTIFIER_STATE;
	mLegalMoves[IDENTIFIER_STATE][DIGIT_CHAR] = IDENTIFIER_STATE;
	// Integers
	mLegalMoves[START_STATE][DIGIT_CHAR] = INTEGER_STATE;
	mLegalMoves[INTEGER_STATE][DIGIT_CHAR]=INTEGER_STATE;
	// Single character tokens
	mLegalMoves[START_STATE][WHITESPACE_CHAR] = START_STATE;
	mLegalMoves[START_STATE][LPAREN_CHAR] = LPAREN_STATE;
	mLegalMoves[START_STATE][RPAREN_CHAR] = RPAREN_STATE;
	mLegalMoves[START_STATE][LCURLY_CHAR] = LCURLY_STATE;
	mLegalMoves[START_STATE][RCURLY_CHAR] = RCURLY_STATE;
	mLegalMoves[START_STATE][SEMICOLON_CHAR] = SEMICOLON_STATE;
	mLegalMoves[START_STATE][EQUAL_CHAR] = ASSIGNMENT_STATE;
	mLegalMoves[START_STATE][PLUS_CHAR] = PLUS_STATE;
	mLegalMoves[START_STATE][MINUS_CHAR] = MINUS_STATE;
	mLegalMoves[START_STATE][STAR_CHAR] = TIMES_STATE;
	mLegalMoves[START_STATE][DIVIDE_CHAR] = DIVIDE_STATE;
	mLegalMoves[START_STATE][LESS_CHAR] = LESS_STATE;
	mLegalMoves[START_STATE][GREATER_CHAR] = GREATER_STATE;
	mLegalMoves[START_STATE][NOT_CHAR] = NOT_STATE;
	mLegalMoves[START_STATE][COMMA_CHAR] = COMMA_STATE;
	mLegalMoves[START_STATE][AND_CHAR] = BITAND_STATE;
	mLegalMoves[START_STATE][OR_CHAR] = BITOR_STATE;

	

	// Comments
	mLegalMoves[DIVIDE_STATE][STAR_CHAR] = COMMENT_STATE;
	for (int i = 0;i < LAST_CHAR;i++) {
		mLegalMoves[COMMENT_STATE][i] = COMMENT_STATE;
	}
	mLegalMoves[COMMENT_STATE][STAR_CHAR] = COMMENTSTAR_STATE;
	for (int i = 0;i < LAST_CHAR;i++) {
		mLegalMoves[COMMENTSTAR_STATE][i] = COMMENT_STATE;
	}
	mLegalMoves[COMMENTSTAR_STATE][STAR_CHAR] = COMMENTSTAR_STATE;
	mLegalMoves[COMMENTSTAR_STATE][DIVIDE_CHAR] = START_STATE;//FULLCOMMENT_STATE;
	// Line comments
	mLegalMoves[DIVIDE_STATE][DIVIDE_CHAR] = LINECOMMENT_STATE;
	for (int i = 0;i < LAST_CHAR;i++) {
		mLegalMoves[LINECOMMENT_STATE][i] = LINECOMMENT_STATE;
	}
	mLegalMoves[LINECOMMENT_STATE][NEWLINE_CHAR] = START_STATE;//FULLCOMMENT_STATE;
	// Two character tokens
	mLegalMoves[BITAND_STATE][AND_CHAR] = AND_STATE;
	mLegalMoves[BITOR_STATE][OR_CHAR] = OR_STATE;

	mLegalMoves[PLUS_STATE][EQUAL_CHAR] = PLUSEQUAL_STATE;
	mLegalMoves[MINUS_STATE][EQUAL_CHAR] = MINUSEQUAL_STATE;
	mLegalMoves[TIMES_STATE][EQUAL_CHAR] = TIMESEQUAL_STATE;
	mLegalMoves[DIVIDE_STATE][EQUAL_CHAR] = DIVIDEEQUAL_STATE;

	mLegalMoves[PLUS_STATE][PLUS_CHAR] = INCREMENT_STATE;
	mLegalMoves[MINUS_STATE][MINUS_CHAR] = DECREMENT_STATE;
	mLegalMoves[TIMES_STATE][STAR_CHAR] = POWER_STATE;

	mLegalMoves[LESS_STATE][LESS_CHAR] = INSERTION_STATE;
	mLegalMoves[LESS_STATE][EQUAL_CHAR] = LESSEQUAL_STATE;

	mLegalMoves[GREATER_STATE][GREATER_CHAR] = EXTRACTION_STATE;
	mLegalMoves[GREATER_STATE][EQUAL_CHAR] = GREATEREQUAL_STATE;

	mLegalMoves[ASSIGNMENT_STATE][EQUAL_CHAR] = EQUAL_STATE;
	mLegalMoves[NOT_STATE][EQUAL_CHAR] = NOTEQUAL_STATE;

	for (int i = 0;i < LAST_STATE;i++) { // Can't move into a eof char
		mLegalMoves[i][EOF_CHAR] = CANTMOVE_STATE;
	}
	for (int i = 0; i < LAST_STATE; i++)
	{
		mCorrespondingTokenTypes[i] = BAD_TOKEN;
	}
	mCorrespondingTokenTypes[IDENTIFIER_STATE] = IDENTIFIER_TOKEN;
	mCorrespondingTokenTypes[INTEGER_STATE] = INTEGER_TOKEN;
	mCorrespondingTokenTypes[LPAREN_STATE] = LPAREN_TOKEN;
	mCorrespondingTokenTypes[RPAREN_STATE] = RPAREN_TOKEN;
	mCorrespondingTokenTypes[LCURLY_STATE] = LCURLY_TOKEN;
	mCorrespondingTokenTypes[RCURLY_STATE] = RCURLY_TOKEN;
	mCorrespondingTokenTypes[SEMICOLON_STATE] = SEMICOLON_TOKEN;
	mCorrespondingTokenTypes[ASSIGNMENT_STATE] = ASSIGNMENT_TOKEN;
	mCorrespondingTokenTypes[LESS_STATE] = LESS_TOKEN;
	mCorrespondingTokenTypes[GREATER_STATE] = GREATER_TOKEN;
	mCorrespondingTokenTypes[SEMICOLON_STATE] = SEMICOLON_TOKEN;
	mCorrespondingTokenTypes[LESSEQUAL_STATE] = LESSEQUAL_TOKEN;
	mCorrespondingTokenTypes[GREATEREQUAL_STATE] = GREATEREQUAL_TOKEN;
	mCorrespondingTokenTypes[INSERTION_STATE] = INSERTION_TOKEN;
	mCorrespondingTokenTypes[EXTRACTION_STATE] = EXTRACTION_TOKEN;
	mCorrespondingTokenTypes[PLUS_STATE] = PLUS_TOKEN;
	mCorrespondingTokenTypes[MINUS_STATE] = MINUS_TOKEN;
	mCorrespondingTokenTypes[TIMES_STATE] = TIMES_TOKEN;
	mCorrespondingTokenTypes[DIVIDE_STATE] = DIVIDE_TOKEN;
	mCorrespondingTokenTypes[FULLCOMMENT_STATE] = COMMENT_TOKEN;
	mCorrespondingTokenTypes[PLUSEQUAL_STATE] = PLUSEQUAL_TOKEN;
	mCorrespondingTokenTypes[MINUSEQUAL_STATE] = MINUSEQUAL_TOKEN;
	mCorrespondingTokenTypes[TIMESEQUAL_STATE] = TIMESEQUAL_TOKEN;
	mCorrespondingTokenTypes[DIVIDEEQUAL_STATE] = DIVIDEEQUAL_TOKEN;
	mCorrespondingTokenTypes[INCREMENT_STATE] = INCREMENT_TOKEN;
	mCorrespondingTokenTypes[DECREMENT_STATE] = DECREMENT_TOKEN;
	mCorrespondingTokenTypes[POWER_STATE] = POWER_TOKEN;
	mCorrespondingTokenTypes[NOTEQUAL_STATE] = NOTEQUAL_TOKEN;
	mCorrespondingTokenTypes[EQUAL_STATE] = EQUAL_TOKEN;
	mCorrespondingTokenTypes[NOT_STATE] = NOT_TOKEN;
	//mCorrespondingTokenTypes[FULLSTRING_STATE] = STRING_TOKEN;
	//mCorrespondingTokenTypes[FULLCHAR_STATE] = CHAR_TOKEN;
	mCorrespondingTokenTypes[COMMA_STATE] = COMMA_TOKEN;
	mCorrespondingTokenTypes[BITAND_STATE] = BITAND_TOKEN;
	mCorrespondingTokenTypes[BITOR_STATE] = BITOR_TOKEN;
	mCorrespondingTokenTypes[AND_STATE] = AND_TOKEN;
	mCorrespondingTokenTypes[OR_STATE] = OR_TOKEN;

}
MachineState StateMachineClass::UpdateState(char currentCharacter, TokenType& correspondingTokenType) {
	correspondingTokenType = mCorrespondingTokenTypes[mCurrentState];
	CharacterType charType = BAD_CHAR;
	// Specific categories
	if (currentCharacter == '\n')
	{
		charType = NEWLINE_CHAR;
	}
	if (charType==BAD_CHAR||mLegalMoves[mCurrentState][charType]==CANTMOVE_STATE) { // Use specific over general if better
		// General categories
		if (isdigit(currentCharacter))
			charType = DIGIT_CHAR;
		else if (isalpha(currentCharacter))
			charType = LETTER_CHAR;
		else if (isspace(currentCharacter))
			charType = WHITESPACE_CHAR;
		else if (currentCharacter == '+')
			charType = PLUS_CHAR;
		else if (currentCharacter == ';')
			charType = SEMICOLON_CHAR;
		else if (currentCharacter == '(')
			charType = LPAREN_CHAR;
		else if (currentCharacter == ')')
			charType = RPAREN_CHAR;
		else if (currentCharacter == '{')
			charType = LCURLY_CHAR;
		else if (currentCharacter == '}')
			charType = RCURLY_CHAR;
		else if (currentCharacter == '=')
			charType = EQUAL_CHAR;
		else if (currentCharacter == '<')
			charType = LESS_CHAR;
		else if (currentCharacter == '>')
			charType = GREATER_CHAR;
		else if (currentCharacter == '/')
			charType = DIVIDE_CHAR;
		else if (currentCharacter == '-')
			charType = MINUS_CHAR;
		else if (currentCharacter == '*')
			charType = STAR_CHAR;
		else if (currentCharacter == EOF)
			charType = EOF_CHAR;
		else if (currentCharacter == '!')
			charType = NOT_CHAR;
		else if (currentCharacter == '\'') {
			charType = SINGLEQUOTE_CHAR;
		}
		else if (currentCharacter == '"') {
			charType = DOUBLEQUOTE_CHAR;
		}
		else if (currentCharacter == '\\') {
			charType = SLASH_CHAR;
		}
		else if (currentCharacter == ',') {
			charType = COMMA_CHAR;
		}
		else if (currentCharacter == '&') {
			charType = AND_CHAR;
		}
		else if (currentCharacter == '|') {
			charType = OR_CHAR;
		}
	}
	MSG("STATE:"<<mCurrentState<<" CHARTYPE:"<<charType<<" CHAR:"<<currentCharacter<<" TOKEN:"<<TokenClass::GetTokenTypeName(correspondingTokenType));
	mCurrentState = mLegalMoves[mCurrentState][charType];
	return mCurrentState;
}
