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
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_PROGRAM:
		{
			fprintf(yyoutSyn, "Rule (PROGRAM -> program VAR_DEFINITIONS; STATEMENTS end)\n");
			parse_VAR_DEFINITIONS();
			match(TOKEN_SEMICOLON);
			parse_STATEMENTS();
			match(TOKEN_END);
			parse_FUNC_DEFINITIONS();
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_PROGRAM] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(PROGRAM);
		}
	}
}

void parse_VAR_DEFINITIONS()
{
	fprintf(yyoutSyn, "Rule (VAR_DEFINITIONS -> VAR_DEFINITION VAR_DEFINITIONS_ TAG)\n");
	parse_VAR_DEFINITION();
	parse_VAR_DEFINITIONS_TAG();
}

void parse_VAR_DEFINITIONS_TAG()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_SEMICOLON:
		{
			Token* secondToken = peek();
			if (secondToken->kind == TOKEN_REAL || secondToken->kind == TOKEN_INTEGER)
			{
				fprintf(yyoutSyn, "Rule (VAR_DEFINITIONS_TAG ->  ;VAR_DEFINITIONS)\n");
				parse_VAR_DEFINITIONS();
			}
			else
			{
				back_token();
				fprintf(yyoutSyn, "Rule (VAR_DEFINITIONS_TAG -> epsilon)\n");
			}

			break;
		}

		case TOKEN_RIGHT_CIRCLE_BRACKET:
		{
			fprintf(yyoutSyn, "Rule (VAR_DEFINITIONS_TAG -> epsilon)\n");
			back_token();
			break; 
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_SEMICOLON, TOKEN_RIGHT_CIRCLE_BRACKET] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(VAR_DEFINITIONS_TAG);
		}
	}
}

void parse_VAR_DEFINITION()
{
	fprintf(yyoutSyn, "Rule (VAR_DEFINITION -> TYPE  VARIABLES_LIST)\n");
	parse_TYPE();
	parse_VARIABLES_LIST();
}


void parse_TYPE()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_REAL:
		{
			fprintf(yyoutSyn, "Rule (TYPE -> real)\n");
			break;
		}

		case TOKEN_INTEGER:
		{
			fprintf(yyoutSyn, "Rule (TYPE -> integer)\n");
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_REAL, TOKEN_INTEGER] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(TYPE);
		}
	}
}

void parse_VARIABLES_LIST()
{
	fprintf(yyoutSyn, "Rule (VARIABLES_LIST -> VARIABLE  VARIABLES_LIST_TAG)\n");
	parse_VARIABLE();
	parse_VARIABLES_LIST_TAG();
}

void parse_VARIABLES_LIST_TAG()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_COMMA:
		{
			fprintf(yyoutSyn, "Rule (VARIABLES_LIST_TAG ->  , VARIABLE VARIABLES_LIST_TAG)\n");
			parse_VARIABLE();
			parse_VARIABLES_LIST_TAG();
			break;
		}

		case TOKEN_SEMICOLON:
		case TOKEN_RIGHT_CIRCLE_BRACKET:
		{
			fprintf(yyoutSyn, "Rule (VARIABLES_LIST_TAG -> epsilon)\n");
			back_token();
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_COMMA, TOKEN_SEMICOLON, TOKEN_RIGHT_CIRCLE_BRACKET] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(VARIABLES_LIST_TAG);
		}
	}
}

void parse_VARIABLE()
{
	fprintf(yyoutSyn, "Rule (VARIABLE -> id VARIABLE_TAG)\n");
	match(TOKEN_ID);
	parse_VARIABLE_TAG();
}

void parse_VARIABLE_TAG()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_LEFT_SQUARE_BRACKET:
		{
			fprintf(yyoutSyn, "Rule (VARIABLE_TAG -> [int_number])\n");
			match(TOKEN_INT_NUMBER);
			match(TOKEN_RIGHT_SQUARE_BRACKET);
			break;
		}

		case TOKEN_SEMICOLON:
		case TOKEN_COMMA:
		case TOKEN_RIGHT_CIRCLE_BRACKET:
		case TOKEN_ASSIGNMENT:
		{
			fprintf(yyoutSyn, "Rule (VARIABLE_TAG -> epsilon)\n");
			back_token();
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_LEFT_SQUARE_BRACKET, TOKEN_SEMICOLON, TOKEN_COMMA, TOKEN_RIGHT_CIRCLE_BRACKET, TOKEN_ASSIGNMENT] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(VARIABLE_TAG);
		}
	}
}

void parse_FUNC_DEFINITIONS()
{
	fprintf(yyoutSyn, "Rule (FUNC_DEFINITIONS -> FUNC_DEFINITION  FUNC_DEFINITIONS_TAG)\n");
	parse_FUNC_DEFINITION();
	parse_FUNC_DEFINITIONS_TAG();
}

void parse_FUNC_DEFINITIONS_TAG()
{
	currentToken = next_token();
	switch (currentToken->kind)
		{
		case TOKEN_REAL:
		case TOKEN_INTEGER:
		case TOKEN_VOID:
		{
			fprintf(yyoutSyn, "Rule (FUNC_DEFINITIONS_TAG - > FUNC_DEFINITION FUNC_DEFINITIONS_TAG)\n");
			back_token();
			parse_FUNC_DEFINITION();
			parse_FUNC_DEFINITIONS_TAG();
			break;
		}

		case TOKEN_EOF:
		{
			fprintf(yyoutSyn, "Rule (FUNC_DEFINITIONS_TAG -> epsilon)\n");
			back_token();
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_REAL, TOKEN_INTEGER, TOKEN_VOID, TOKEN_EOF] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(FUNC_DEFINITIONS_TAG);
		}
	}
}

void parse_FUNC_DEFINITION()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_REAL:
		case TOKEN_INTEGER:
		case TOKEN_VOID:
		{
			fprintf(yyoutSyn, "Rule (FUNC_DEFINITION -> RETURNED_TYPE id (PARAM_DEFINITIONS) BLOCK)\n");
			back_token();
			parse_RETURNED_TYPE();
			match(TOKEN_ID);
			match(TOKEN_LEFT_CIRCLE_BRACKET);
			parse_PARAM_DEFINITIONS();
			match(TOKEN_RIGHT_CIRCLE_BRACKET);
			parse_BLOCK();
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_REAL, TOKEN_INTEGER, TOKEN_VOID] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(FUNC_DEFINITION);
		}
	}
}


void parse_RETURNED_TYPE()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_VOID:
		{
			fprintf(yyoutSyn, "Rule (RETURNED_TYPE -> void)\n");
			break;
		}

		case TOKEN_REAL:
		case TOKEN_INTEGER:
		{
			fprintf(yyoutSyn, "Rule (RETURNED_TYPE -> TYPE)\n");
			back_token();
			parse_TYPE();
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_REAL, TOKEN_INTEGER, TOKEN_VOID] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(RETURNED_TYPE);
		}
	}
}


void parse_PARAM_DEFINITIONS()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_REAL:
		case TOKEN_INTEGER:
		{
			fprintf(yyoutSyn, "Rule (PARAM_DEFINITIONS -> VAR_DEFINITIONS)\n");
			back_token();
			parse_VAR_DEFINITIONS();
			break;
		}

		case TOKEN_RIGHT_CIRCLE_BRACKET:
		{
			fprintf(yyoutSyn, "Rule (PARAM_DEFINITIONS -> epsilon)\n");
			back_token();
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_REAL, TOKEN_INTEGER, TOKEN_RIGHT_CIRCLE_BRACKET] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(PARAM_DEFINITIONS);
		}
	}
}

void parse_STATEMENTS()
{
	currentToken = next_token();
	switch (currentToken->kind)
		{
		case TOKEN_RETURN:
		case TOKEN_ID:
		case TOKEN_LEFT_CURLY_BRACKET:
		{
			fprintf(yyoutSyn, "Rule (STATEMENTS -> STATEMENT;  STATEMENTS_TAG)\n");
			back_token();
			parse_STATEMENT();
			match(TOKEN_SEMICOLON);
			parse_STATEMENTS_TAG();
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_RETURN, TOKEN_ID, TOKEN_LEFT_CURLY_BRACKET] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(STATEMENTS);
		}
	}
}

void parse_STATEMENTS_TAG()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_RETURN:
		case TOKEN_ID:
		case TOKEN_LEFT_CURLY_BRACKET:
		{
			fprintf(yyoutSyn, "Rule (STATEMENTS_TAG -> STATEMENTS)\n");
			back_token();
			parse_STATEMENTS();
			break;
		}

		case TOKEN_END:
		case TOKEN_RIGHT_CURLY_BRACKET:
		{
			fprintf(yyoutSyn, "Rule (STATEMENTS_TAG -> epslion)\n");
			back_token();
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_RETURN, TOKEN_ID, TOKEN_LEFT_CURLY_BRACKET, TOKEN_END, TOKEN_RIGHT_CURLY_BRACKET] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(STATEMENTS_TAG);
		}
	}
}

void parse_STATEMENT()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_RETURN:
		{
			fprintf(yyoutSyn, "Rule (STATEMENT -> return STATEMENT_TAG)\n");
			parse_STATEMENT_TAG();
			break;
		}

		case TOKEN_LEFT_CURLY_BRACKET:
		{
			fprintf(yyoutSyn, "Rule (STATEMENT -> BLOCK)\n");
			back_token();
			parse_BLOCK();
			break;
		}

		case TOKEN_ID:
		{
			fprintf(yyoutSyn, "Rule (STATEMENT ->  id STATEMENT_TAG_2)\n");
			parse_STATEMENT_TAG_2();
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_RETURN, TOKEN_ID, TOKEN_LEFT_CURLY_BRACKET] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(STATEMENT);
		}
	}
}

void parse_STATEMENT_TAG()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_REAL_NUMBER:
		case TOKEN_INT_NUMBER:
		case TOKEN_ID:
		{
			fprintf(yyoutSyn, "Rule (STATEMENT_TAG - > EXPRESSION)\n");
			back_token();
			parse_EXPRESSION();
			break;
		}

		case TOKEN_SEMICOLON:
		{
			fprintf(yyoutSyn, "Rule (STATEMENT_TAG - > epsilon)\n");
			back_token();
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_REAL_NUMBER, TOKEN_ID, TOKEN_INT_NUMBER, TOKEN_SEMICOLON] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(STATEMENT_TAG);
		}
	}
}

void parse_STATEMENT_TAG_2()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_LEFT_SQUARE_BRACKET:
		{
			fprintf(yyoutSyn, "Rule (STATEMENT_TAG_2 -> VARIABLE_TAG = EXPRESSION)\n");
			back_token();
			parse_VARIABLE_TAG();
			match(TOKEN_ASSIGNMENT);
			parse_EXPRESSION();
			break;
		}

		case TOKEN_ASSIGNMENT:
		{
			fprintf(yyoutSyn, "Rule (STATEMENT_TAG_2 -> VARIABLE_TAG = EXPRESSION)\n");
			fprintf(yyoutSyn, "Rule (VARIABLE_TAG -> epsilon)\n");
			parse_EXPRESSION();
			break;
		}

		case TOKEN_LEFT_CIRCLE_BRACKET:
		{
			fprintf(yyoutSyn, "Rule (STATEMENT_TAG_2 -> (PARMETERS_LIST))\n");
			parse_PARMETERS_LIST();
			match(TOKEN_RIGHT_CIRCLE_BRACKET);
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_LEFT_SQUARE_BRACKET, TOKEN_LEFT_CIRCLE_BRACKET, TOKEN_ASSIGNMENT] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(STATEMENT_TAG_2);
		}
	}
}

void parse_BLOCK()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_LEFT_CURLY_BRACKET:
		{
			fprintf(yyoutSyn, "Rule (BLOCK -> { VAR_DEFINITIONS; STATEMENTS })\n");
			parse_VAR_DEFINITIONS();
			match(TOKEN_SEMICOLON);
			parse_STATEMENTS();
			match(TOKEN_RIGHT_CURLY_BRACKET);
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_LEFT_CURLY_BRACKET] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(BLOCK);
		}

	}
}

void parse_PARMETERS_LIST()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_ID:
		{
			fprintf(yyoutSyn, "Rule (PARMETERS_LIST - > VARIABLES_LIST)\n");
			back_token();
			parse_VARIABLES_LIST();
			break;
		}

		case TOKEN_RIGHT_CIRCLE_BRACKET:
		{
			fprintf(yyoutSyn, "Rule (PARMETERS_LIST - > epsilon)\n");
			back_token();
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_ID, TOKEN_RIGHT_CIRCLE_BRACKET] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(PARMETERS_LIST);
		}
	}
}

void parse_EXPRESSION()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_INT_NUMBER:
		{
			fprintf(yyoutSyn, "Rule (EXPRESSION -> int_number)\n");
			break;
		}

		case TOKEN_REAL_NUMBER:
		{
			fprintf(yyoutSyn, "Rule (EXPRESSION -> real_number)\n");
			break;
		}

		case TOKEN_ID:
		{
			fprintf(yyoutSyn, "Rule (EXPRESSION -> id EXPRESSION_TAG)\n");
			parse_EXPRESSION_TAG();
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_ID, TOKEN_INT_NUMBER, TOKEN_REAL_NUMBER] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(EXPRESSION);
		}
	}
}

void parse_EXPRESSION_TAG()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_MUL:
		case TOKEN_DIV:
		{
			fprintf(yyoutSyn, "Rule (EXPRESSION_TAG -> * EXPRESSION | / EXPRESSION)\n");
			parse_EXPRESSION();
			break;
		}

		case TOKEN_LEFT_SQUARE_BRACKET:
		{
			fprintf(yyoutSyn, "Rule (EXPRESSION_TAG -> VARIABLE_TAG)\n");
			back_token();
			parse_VARIABLE_TAG();
			break;
		}

		case TOKEN_SEMICOLON:
		{
			fprintf(yyoutSyn, "Rule (EXPRESSION_TAG -> ellipse)\n");
			back_token();
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_MUL, TOKEN_DIV, TOKEN_REAL_NUMBER, TOKEN_LEFT_SQUARE_BRACKET, TOKEN_SEMICOLON] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(EXPRESSION_TAG);
		}
	}
}


// This function check if token is follow of variable
int isFollowOfVariable(Grammer variable, eTOKENS kind)
{
	switch (variable)
	{
		case PROGRAM:
		case FUNC_DEFINITIONS:
		case FUNC_DEFINITIONS_TAG:
		{
			switch (kind)
			{
			case TOKEN_EOF: return 1;
			default: return 0;
			}
		}

		case VAR_DEFINITIONS:
		case VAR_DEFINITIONS_TAG:
		case VAR_DEFINITION:
		case VARIABLES_LIST:
		case VARIABLES_LIST_TAG:
		{
			switch (kind)
			{
			case TOKEN_SEMICOLON:
			case TOKEN_RIGHT_CIRCLE_BRACKET: return 1;
			default: return 0;
			}
		}

		case TYPE:
		case RETURNED_TYPE:
		{
			switch (kind)
			{
			case TOKEN_ID: return 1;
			default: return 0;
			}
		}

		case VARIABLE:
		{
			switch (kind)
			{
			case TOKEN_COMMA:
			case TOKEN_RIGHT_CIRCLE_BRACKET:
			case TOKEN_SEMICOLON: return 1;
			default: return 0;
			}
		}

		case VARIABLE_TAG:
		{
			switch (kind)
			{
			case TOKEN_ASSIGNMENT:
			case TOKEN_COMMA:
			case TOKEN_RIGHT_CIRCLE_BRACKET:
			case TOKEN_SEMICOLON: return 1;
			default: return 0;
			}
		}

		case FUNC_DEFINITION:
		{
			switch (kind)
			{
			case TOKEN_VOID:
			case TOKEN_REAL:
			case TOKEN_INTEGER:
			case TOKEN_EOF: return 1;
			default: return 0;
			}
		}

		case PARAM_DEFINITIONS:
		case PARMETERS_LIST:
		{
			switch (kind)
			{
			case TOKEN_RIGHT_CIRCLE_BRACKET: return 1;
			default: return 0;
			}
		}

		case STATEMENTS:
		case STATEMENTS_TAG:
		{
			switch (kind)
			{
			case TOKEN_RIGHT_CURLY_BRACKET:
			case TOKEN_END: return 1;
			default: return 0;
			}
		}

		case STATEMENT:
		case STATEMENT_TAG:
		case STATEMENT_TAG_2:
		case EXPRESSION:
		case EXPRESSION_TAG:
		{
			switch (kind)
			{
			case TOKEN_SEMICOLON: return 1;
			default: return 0;
			}
		}

		case BLOCK:
		{
			switch (kind)
			{
			case TOKEN_VOID:
			case TOKEN_REAL:
			case TOKEN_INTEGER:
			case TOKEN_EOF:
			case TOKEN_SEMICOLON: return 1;
			default: return 0;
			}
		}

	default: return 0;

	}
}

// This function helps the compiler recover in the event of an error
void recoveryFromError(Grammer i_Variable)
{
	while (!isFollowOfVariable(i_Variable, currentToken->kind) && currentToken->kind != TOKEN_EOF)
	{
		currentToken = next_token();
	}

	back_token();
}


