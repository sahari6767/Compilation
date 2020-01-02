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

void mainParser()
{
	fprintf(yyoutSyn, "Parser starting\n");
	parse_PROGRAM();
	match(TOKEN_EOF);
	fprintf(yyoutSyn, "Parser finshed\n");
}

void parse_PROGRAM()
{
	fprintf(yyoutSyn, "Rule (PROGRAM -> BLOCK)\n");
	parse_BLOCK();
}

void parse_BLOCK()
{
	currentToken = next_token();
	switch(currentToken->kind)
	{
		case TOKEN_BLOCK:
		{
			fprintf(yyoutSyn, "Rule (BLOCK -> block DECLARATIONS begin STATEMENTS end)\n");
			parse_DECLARATIONS();
			match(TOKEN_BEGIN);
			parse_STATEMENTS();
			match(TOKEN_END);
			break;
		}
		
		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_BLOCK] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(BLOCK);
		}
	}

}

void parse_DECLARATIONS()
{
	fprintf(yyoutSyn, "Rule (DECLARATIONS -> DECLARATION DECLARATIONS_NEW)\n");
	parse_DECLARATION();
	parse_DECLARATIONS_NEW();

}

void parse_DECLARATIONS_NEW()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_SEMICOLON:
		{
			Token* secondToken = peek();//WTF IS THAT??
			if (secondToken->kind == TOKEN_ID || secondToken->kind == TOKEN_TYPE)
			{
				fprintf(yyoutSyn, "Rule (DECLARATIONS_NEW -> ; DECLARATIONS)\n");
				parse_DECLARATIONS();
			}
			else
			{
				back_token();
				fprintf(yyoutSyn, "Rule (DECLARATIONS_NEW -> epsilon)\n");
			}

			break;
		}

		case TOKEN_RIGHT_PARENTHESES:
		{
			fprintf(yyoutSyn, "Rule (DECLARATIONS_NEW -> epsilon)\n");
			back_token();
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_SEMICOLON, TOKEN_RIGHT_PARENTHESES] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(DECLARATIONS_NEW);
		}
	}
}

void parse_DECLARATION()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_ID:
		{
			fprintf(yyoutSyn, "Rule (DECLARATION -> VAR_DECLARATION)\n");
			back_token();
			parse_VAR_DECLARATION();
			break;
		}
		
		case TOKEN_TYPE:
		{
			fprintf(yyoutSyn, "Rule (DECLARATION -> TYPE_DECLARATION)\n");
			back_token();
			parse_TYPE_DECLARATION();
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_REAL, TOKEN_INTEGER, TOKEN_RIGHT_CIRCLE_BRACKET] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(DECLARATION);
		}
	}

}

void parse_VAR_DECLARATION()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_ID:
		{
			fprintf(yyoutSyn, "Rule (VAR_DECLARATION -> id : VAR_DECLARATION_NEW)\n");
			match(TOKEN_COLON);
			parse_VAR_DECLARATION_NEW();
			break;
		}
		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_ID] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(VAR_DECLARATION);
		}
	}
}

void parse_VAR_DECLARATION_NEW()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_INTEGER:
		{
			fprintf(yyoutSyn, "Rule (VAR_DECLARATION_NEW -> SIMPLE_TYPE)\n"); // Is SIMPLE_TYPE Neccecery?
			parse_SIMPLE_TYPE();
			break;
		}
	
		case TOKEN_REAL:
		{
			fprintf(yyoutSyn, "Rule (VAR_DECLARATION_NEW -> SIMPLE_TYPE)\n"); // Is SIMPLE_TYPE Neccecery?
			parse_SIMPLE_TYPE();
			break;
		}
	
		case TOKEN_ARRAY:
		{
			fprintf(yyoutSyn, "VAR_DECLARATION_NEW -> array [SIZE] of SIMPLE_TYPE)\n"); 
			match(TOKEN_LEFT_BRACKETS);
			parse_SIZE();
			match(TOKEN_RIGHT_BRACKETS);
			match(TOKEN_OF);
			parse_SIMPLE_TYPE();
			break;
		}
	
		case TOKEN_TYPE_NAME:
		{
			fprintf(yyoutSyn, "VAR_DECLARATION_NEW -> type_name)\n"); 
		}
		
		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_INTEGER,TOKEN_REAL,TOKEN_ARRAY,TOKEN_TYPE_NAME] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(VAR_DECLARATION_NEW);
		}
	}
}


