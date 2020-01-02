#include "Token.h"
#include "Parser.h"
#define _CRT_SECURE_NO_WARNINGS

extern FILE *yyoutLex, *yyoutSyn;
extern Node* currentNode;
extern Node* headNode;
Token *currentToken;

void match(eTOKENS i_ExpectedTokenKind)
{
	currentToken = next_token();
	if (currentToken->kind != i_ExpectedTokenKind)
	{
		fprintf(yyoutSyn, "Expected token of type {%s} at line: {%d}, Actual token of type {%s}, lexeme: {%s}\n", convertFromTokenKindToString(i_ExpectedTokenKind), currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
		back_token();
	}
}

