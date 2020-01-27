#pragma once
#include "Token.h"
#include "SymbolTable.h"
#define _CRT_SECURE_NO_WARNINGS

typedef enum Grammer
{
	PROGRAM,
	VAR_DEFINITIONS,
	VAR_DEFINITIONS_TAG,
	VAR_DEFINITION,
	TYPE,
	VARIABLES_LIST,
	VARIABLES_LIST_TAG,
	VARIABLE,
	VARIABLE_TAG,
	FUNC_DEFINITIONS,
	FUNC_DEFINITIONS_TAG,
	FUNC_DEFINITION,
	RETURNED_TYPE,
	PARAM_DEFINITIONS,
	STATEMENTS,
	STATEMENTS_TAG,
	STATEMENT,
	STATEMENT_TAG,
	STATEMENT_TAG_2,
	BLOCK,
	PARMETERS_LIST,
	EXPRESSION,
	EXPRESSION_TAG

}Grammer;

void match(eTOKENS i_ExpectedTokenKind);
void recoveryFromError(Grammer i_Variable);
void mainParser();
void PrintAllVariableThatNeverUsed(SymTable* current_ptr);
void parse_PROGRAM();
void parse_VAR_DEFINITIONS();
void parse_VAR_DEFINITIONS_TAG();
void parse_VAR_DEFINITION();
int parse_TYPE();
void parse_VARIABLES_LIST(int type);
void parse_VARIABLES_LIST_TAG(int type);
void parse_VARIABLE(int type);
void parse_VARIABLE_TAG(int fromWhichParser);

void parse_FUNC_DEFINITIONS();
void parse_FUNC_DEFINITIONS_TAG();
void parse_FUNC_DEFINITION();
void parse_RETURNED_TYPE();
void parse_PARAM_DEFINITIONS();
void parse_STATEMENTS();
void parse_STATEMENTS_TAG();
void parse_STATEMENT();
void parse_STATEMENT_TAG();
void parse_STATEMENT_TAG_2(char *name);
void parse_BLOCK();
void parse_PARMETERS_LIST(char *name);
void parse_EXPRESSION(int flag);
void parse_EXPRESSION_TAG(int flag);



