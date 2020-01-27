#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include "Token.h"
#include "Parser.h"
#include "SymbolTable.h"


extern FILE *yyoutLex, *yyoutSyn, *yyoutSem;
extern Node* currentNode;
extern Node* headNode;
Token *currentToken;
SymTableEntry *tempEntry = NULL;
int countParmeters;


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
	resetFunctionsData();
	countParmeters = 0;
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
		cur_table = make_table(NULL);
		fprintf(yyoutSyn, "Rule (PROGRAM -> program VAR_DEFINITIONS; STATEMENTS end)\n");
		parse_VAR_DEFINITIONS();
		match(TOKEN_SEMICOLON);
		parse_STATEMENTS();
		match(TOKEN_END);
		PrintAllVariableThatNeverUsed(cur_table);
		cur_table = pop_table(cur_table);
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

void parse_VAR_DEFINITION()
{
	int type;
	fprintf(yyoutSyn, "Rule (VAR_DEFINITION -> TYPE  VARIABLES_LIST)\n");
	type = parse_TYPE();
	parse_VARIABLES_LIST(type);
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


int parse_TYPE()
{
	int type;
	currentToken = next_token();
	switch (currentToken->kind)
	{
	case TOKEN_REAL:
	{
		fprintf(yyoutSyn, "Rule (TYPE -> real)\n");
		type = REAL;
		break;
	}

	case TOKEN_INTEGER:
	{
		fprintf(yyoutSyn, "Rule (TYPE -> integer)\n");
		type = INTEGER;
break;
	}

	default:
	{
		fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_REAL, TOKEN_INTEGER] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
		recoveryFromError(TYPE);
		type = 0; 
	}
	}

	return type;
}

void parse_VARIABLES_LIST(int type)
{
	fprintf(yyoutSyn, "Rule (VARIABLES_LIST -> VARIABLE  VARIABLES_LIST_TAG)\n");
	parse_VARIABLE(type);
	parse_VARIABLES_LIST_TAG(type);
}

void parse_VARIABLES_LIST_TAG(int type)
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
	case TOKEN_COMMA:
	{
		fprintf(yyoutSyn, "Rule (VARIABLES_LIST_TAG ->  , VARIABLE VARIABLES_LIST_TAG)\n");
		parse_VARIABLE(type);
		parse_VARIABLES_LIST_TAG(type);
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

void parse_VARIABLE(int type)
{
	fprintf(yyoutSyn, "Rule (VARIABLE -> id VARIABLE_TAG)\n");
	match(TOKEN_ID);
	if (type != 0)
	{
		if (!(strcmp(funcArray[index].name, "") == 0))
		{
			if (findFunction(currentToken->lexeme) == 1) {
				fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: '%s' Function can't have a local variable with the same name as its parameter\n", "C016",
					currentToken->lineNumber, currentToken->lexeme);
			}

			if (isIdExistInFunction(currentToken->lexeme) == 1)
			{
				fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: id '%s' already defined in function declertion\n", "C013",
					currentToken->lineNumber, currentToken->lexeme);
			}
			else
			{
				cur_entry = insert(currentToken->lexeme, cur_table);
				if (cur_entry == NULL)
				{
					fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: The variable %s already defined\n", "C001",
						currentToken->lineNumber, currentToken->lexeme);
				}
				else
				{
					set_type(cur_entry, type);
					set_subType(cur_entry, type);
					set_roleType(cur_entry, VAR);
					setLineNumber(cur_entry, currentToken->lineNumber);
				}
			}
		}
		else
		{
			cur_entry = insert(currentToken->lexeme, cur_table);
			if (cur_entry == NULL)
			{
				fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: The variable %s already defined\n", "C001",
					currentToken->lineNumber, currentToken->lexeme);
			}
			else
			{
				set_type(cur_entry, type);
				set_subType(cur_entry, type);
				set_roleType(cur_entry, VAR);
				setLineNumber(cur_entry, currentToken->lineNumber);
			}
		}
	}
	else
	{
		countParmeters++;
		if (find(currentToken->lexeme, cur_table) == NULL)
		{
			fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: The variable '%s' isn't defined\n", "C004",
				currentToken->lineNumber, currentToken->lexeme);
		}
	}

	parse_VARIABLE_TAG(VARIABLE);
}

void parse_VARIABLE_TAG(int fromWhichParser)
{
	int arraySizeIndex = -1;
	int arraySize;
	currentToken = next_token();
	switch (currentToken->kind)
	{
	case TOKEN_LEFT_SQUARE_BRACKET:
	{
		fprintf(yyoutSyn, "Rule (VARIABLE_TAG -> [int_number])\n");
		match(TOKEN_INT_NUMBER);
		arraySizeIndex = atoi(currentToken->lexeme);
		switch (fromWhichParser)
		{
		case VARIABLE: // define new variable (set size array and type)
			set_size(cur_entry, arraySizeIndex);
			set_subType(cur_entry, ARRAY);
			set_roleType(cur_entry, VAR);
			setLineNumber(cur_entry, currentToken->lineNumber);
			break;

		case STATEMENT_TAG_2: // assigment to array
			if (cur_entry && cur_entry->subType == ARRAY)
			{
				arraySize = cur_entry->size;
				if (arraySizeIndex >= arraySize)
				{
					fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: The array '%s' index out of bound\n", "C003",
						currentToken->lineNumber, cur_entry->name);
				}
			}
			
			if (cur_entry && cur_entry->subType != ARRAY)
			{
				fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: The vairable '%s' is not an array\n", "C005",
					currentToken->lineNumber, cur_entry->name);
			}

			break;

		case EXPRESSION_TAG:
			if (tempEntry && tempEntry->size <= arraySizeIndex)
			{
				setError(ARRAY_INDEX_OUT_OF_RANGE, currentToken->lineNumber, tempEntry->name);
			}

			printErrors(currentToken->lineNumber);
			break;
		default:
			break;
		}
		
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

void parse_FUNC_DEFINITION()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
	case TOKEN_REAL:
	{
		fprintf(yyoutSyn, "Rule (FUNC_DEFINITION -> RETURNED_TYPE id (PARAM_DEFINITIONS) BLOCK)\n");
		back_token();
		parse_RETURNED_TYPE();
		match(TOKEN_ID);

		if (ifFunctionExist(currentToken->lexeme))
		{
			fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: Function '%s' Already Defined\n", "C015",
				currentToken->lineNumber, currentToken->lexeme);
		}
		else
		{
			funcArray[index].name = currentToken->lexeme;
			funcArray[index].returnType = REAL;
			funcArray[index].lineNumber = currentToken->lineNumber;
		}



		match(TOKEN_LEFT_CIRCLE_BRACKET);
		cur_table = make_table(NULL);
		parse_PARAM_DEFINITIONS();
		howMuchVariables();
		match(TOKEN_RIGHT_CIRCLE_BRACKET);
	
		parse_BLOCK();
		cur_table = pop_table(cur_table);
		index++;
		break;
	}

	case TOKEN_INTEGER:
	{
		fprintf(yyoutSyn, "Rule (FUNC_DEFINITION -> RETURNED_TYPE id (PARAM_DEFINITIONS) BLOCK)\n");
		back_token();
		parse_RETURNED_TYPE();
		match(TOKEN_ID);


		if (ifFunctionExist(currentToken->lexeme))
		{
			fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: Function '%s' Already Defined\n", "C015",
				currentToken->lineNumber, currentToken->lexeme);
		}
		else
		{
			funcArray[index].name = currentToken->lexeme;
			funcArray[index].returnType = INTEGER;
			funcArray[index].lineNumber = currentToken->lineNumber;
		}


		match(TOKEN_LEFT_CIRCLE_BRACKET);
		cur_table = make_table(NULL);
		parse_PARAM_DEFINITIONS();
		howMuchVariables();
		match(TOKEN_RIGHT_CIRCLE_BRACKET);
	
		parse_BLOCK();
		cur_table = pop_table(cur_table);
		index++;
		break;
	}

	case TOKEN_VOID:
	{
		fprintf(yyoutSyn, "Rule (FUNC_DEFINITION -> RETURNED_TYPE id (PARAM_DEFINITIONS) BLOCK)\n");
		back_token();
		parse_RETURNED_TYPE();
		match(TOKEN_ID);

		if (ifFunctionExist(currentToken->lexeme))
		{
			fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: Function '%s' Already Defined\n", "C015",
				currentToken->lineNumber, currentToken->lexeme);
		}
		else
		{
			funcArray[index].name = currentToken->lexeme;
			funcArray[index].returnType = VOID;
			funcArray[index].lineNumber = currentToken->lineNumber;
		}

		match(TOKEN_LEFT_CIRCLE_BRACKET);
		cur_table = make_table(NULL);
		parse_PARAM_DEFINITIONS();
		howMuchVariables();
		match(TOKEN_RIGHT_CIRCLE_BRACKET);
		parse_BLOCK();
		cur_table = pop_table(cur_table);
		index++;
		break;
	}

	default:
	{
		fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_REAL, TOKEN_INTEGER, TOKEN_VOID] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
		recoveryFromError(FUNC_DEFINITION);
	}
	}
}

void parse_FUNC_DEFINITIONS_TAG()
{
	int type = -1;
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
	case TOKEN_ID:
	{
		fprintf(yyoutSyn, "Rule (STATEMENTS -> STATEMENT;  STATEMENTS_TAG)\n");
		back_token();
		parse_STATEMENT();
		match(TOKEN_SEMICOLON);
		parse_STATEMENTS_TAG();
		break;
	}
	case TOKEN_RETURN:
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
		

		parse_STATEMENT_TAG_2(currentToken->lexeme);
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
	{
		fprintf(yyoutSyn, "Rule (STATEMENT_TAG - > EXPRESSION)\n");
		if (strcmp(funcArray[index].name, "") == 0)
		{

			fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: return statement in the main can't return a value\n", "C017",
				currentToken->lineNumber);
		}
		else
		{
			if (funcArray[index].returnType == VOID)
			{
				fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: Function of type void can't return values\n", "C018",
					currentToken->lineNumber);
			}
			else if (funcArray[index].returnType == INTEGER)
			{
				fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: Function of type Integer can't return Real value\n", "C019",
					currentToken->lineNumber);
			}
		}

		back_token();
		parse_EXPRESSION(1);
		break;
	}

	case TOKEN_INT_NUMBER:
	{
		fprintf(yyoutSyn, "Rule (STATEMENT_TAG - > EXPRESSION)\n");
		if (strcmp(funcArray[index].name, "") == 0)
		{

			fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: return statement in the main can't return a value\n", "C017",
				currentToken->lineNumber);
		}
		else
		{
			if (funcArray[index].returnType == VOID)
			{

				fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: Function of type void can't return values\n", "C018",
					currentToken->lineNumber);
			}
		}

		back_token();
		parse_EXPRESSION(1);
		break;
	}

	case TOKEN_ID:
	{
		fprintf(yyoutSyn, "Rule (STATEMENT_TAG - > EXPRESSION)\n");
		if (strcmp(funcArray[index].name, "") == 0)
		{

			fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: return statement in the main can't return a value\n", "C017",
				currentToken->lineNumber);
		}
		else
		{
			if (funcArray[index].returnType == VOID)
			{

				fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: Function of type void can't return values\n", "C018",
					currentToken->lineNumber);
			}
		}

		back_token();
		parse_EXPRESSION(1);
		break;
	}

	case TOKEN_SEMICOLON:
	{
		fprintf(yyoutSyn, "Rule (STATEMENT_TAG - > epsilon)\n");
		if (!strcmp(funcArray[index].name, "") == 0) 
		{
			if (funcArray[index].returnType == REAL || funcArray[index].returnType == INTEGER)
			{
				fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: Function must return a value\n", "C019",
					currentToken->lineNumber);
			}
		}

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

void parse_STATEMENT_TAG_2(char* id)
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
	case TOKEN_LEFT_SQUARE_BRACKET:
	{
		fprintf(yyoutSyn, "Rule (STATEMENT_TAG_2 -> VARIABLE_TAG = EXPRESSION)\n");
		cur_entry = find(id, cur_table);
		if (cur_entry == NULL)
		{
			fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: The variable '%s' isn't defined\n", "C004",
				currentToken->lineNumber, id);
		}
		else
		{
			cur_entry->countInstance = cur_entry->countInstance + 1;
		}
		back_token();
		parse_VARIABLE_TAG(STATEMENT_TAG_2);
		match(TOKEN_ASSIGNMENT);
		parse_EXPRESSION(2); 
		break;
	}

	case TOKEN_ASSIGNMENT:
	{
		fprintf(yyoutSyn, "Rule (STATEMENT_TAG_2 -> VARIABLE_TAG = EXPRESSION)\n");
		fprintf(yyoutSyn, "Rule (VARIABLE_TAG -> epsilon)\n");

		cur_entry = find(id, cur_table);
		if (cur_entry == NULL)
		{
			fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: The variable '%s' isn't defined\n", "C004",
				currentToken->lineNumber, id);
		}
		else
		{
			cur_entry->countInstance = cur_entry->countInstance + 1;
		}

		if (cur_entry && cur_entry->subType == ARRAY)
		{
			fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: %s\n","C002",
				currentToken->lineNumber,"You can't assign to entire array");
		}
		parse_EXPRESSION(2); 
		break;
	}

	case TOKEN_LEFT_CIRCLE_BRACKET:
	{
		fprintf(yyoutSyn, "Rule (STATEMENT_TAG_2 -> (PARMETERS_LIST))\n");
		if (findFunction(id) == 0)
		{
			fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: Function is not define yet\n", "C014",
				currentToken->lineNumber);
		}

		parse_PARMETERS_LIST(id);
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
		cur_table = make_table(cur_table);
		fprintf(yyoutSyn, "Rule (BLOCK -> { VAR_DEFINITIONS; STATEMENTS })\n");
		parse_VAR_DEFINITIONS();
		match(TOKEN_SEMICOLON);
		parse_STATEMENTS();
		match(TOKEN_RIGHT_CURLY_BRACKET);
		PrintAllVariableThatNeverUsed(cur_table); // before pop the current tabel print all variable that never used.
		cur_table = pop_table(cur_table);
		break;
	}

	default:
	{
		fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_LEFT_CURLY_BRACKET] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
		recoveryFromError(BLOCK);
	}

	}
}

void parse_PARMETERS_LIST(char * id)
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
	case TOKEN_ID:
	{
		fprintf(yyoutSyn, "Rule (PARMETERS_LIST - > VARIABLES_LIST)\n");
		back_token();
		parse_VARIABLES_LIST(0);
		if (findFunction(id) == 1)
		{
			int indx = getIndexFunction(id);
			if (indx != -1)
			{
				if (countParmeters != funcArray[indx].totalVariables)
				{
					fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: Invalid number of parameters in function call\n", "C012",
						currentToken->lineNumber);
				}
			}
			
		}
		countParmeters = 0;
		break;
	}

	case TOKEN_RIGHT_CIRCLE_BRACKET:
	{
		fprintf(yyoutSyn, "Rule (PARMETERS_LIST - > epsilon)\n");
		back_token();
		if (findFunction(id) == 1)
		{
			int indx = getIndexFunction(id);
			if (indx != -1)
			{
				if (countParmeters != funcArray[indx].totalVariables)
				{
					fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: Invalid number of parameters in function call\n", "C012",
					currentToken->lineNumber);
				}
			}

		}
		countParmeters = 0;
		break;
	}

	default:
	{
		fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_ID, TOKEN_RIGHT_CIRCLE_BRACKET] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
		recoveryFromError(PARMETERS_LIST);
	}
	}
}

void parse_EXPRESSION(int flag)
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
	case TOKEN_INT_NUMBER:
	{
		fprintf(yyoutSyn, "Rule (EXPRESSION -> int_number)\n");
		printErrors(currentToken->lineNumber);

		break;
	}

	case TOKEN_REAL_NUMBER:
	{
		fprintf(yyoutSyn, "Rule (EXPRESSION -> real_number)\n");
		if (cur_entry && cur_entry->type == INTEGER && flag != 1)
		{
			char currentLexeme[100];
			char allLexeme[100];
			strcpy(currentLexeme, currentToken->lexeme);
			strcpy(allLexeme, cur_entry->errorsExpressions[REAL_TO_INTEGER].variableName);
			strcat(allLexeme, currentLexeme);
			strcat(allLexeme, ", ");
			setError(REAL_TO_INTEGER, currentToken->lineNumber, allLexeme);
			flag = 0;
		}

		printErrors(currentToken->lineNumber);
		break;
	}

	case TOKEN_ID:
	{
		fprintf(yyoutSyn, "Rule (EXPRESSION -> id EXPRESSION_TAG)\n");
		tempEntry = find(currentToken->lexeme, cur_table);
		if (tempEntry == NULL && cur_entry)
		{
			char currentLexeme[100];
			char allLexeme[100];
			strcpy(currentLexeme, currentToken->lexeme);
			strcpy(allLexeme, cur_entry->errorsExpressions[RIGHT_VARIABLE_UNDEFINED].variableName);
			strcat(allLexeme, currentLexeme);
			strcat(allLexeme, ", ");
			setError(RIGHT_VARIABLE_UNDEFINED, currentToken->lineNumber, allLexeme);
			flag = 0;
		}
		else if (tempEntry == NULL && cur_entry == NULL)
		{
			fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: Both sides of the expression are undfined\n", "C011",
				currentToken->lineNumber);
			flag = 0;
		}
		else
		{
			if (cur_entry && cur_entry->type == INTEGER && tempEntry->type == REAL && flag != 1)
			{
				char currentLexeme[100];
				char allLexeme[100];
				strcpy(currentLexeme, currentToken->lexeme);
				strcpy(allLexeme, cur_entry->errorsExpressions[REAL_TO_INTEGER].variableName);
				strcat(allLexeme, currentLexeme);
				strcat(allLexeme, ", ");
				setError(REAL_TO_INTEGER, currentToken->lineNumber, allLexeme);
				flag = 0;
			}
		}

		if (tempEntry && tempEntry->subType == ARRAY && tempEntry->type == REAL && cur_entry && cur_entry->type == INTEGER)
		{
			flag = 0;
			char currentLexeme[100];
			char allLexeme[100];
			strcpy(currentLexeme, currentToken->lexeme);
			strcpy(allLexeme, cur_entry->errorsExpressions[REAL_TO_INTEGER].variableName);
			strcat(allLexeme, currentLexeme);
			strcat(allLexeme, ", ");
			setError(REAL_TO_INTEGER, currentToken->lineNumber, allLexeme);
		}

		parse_EXPRESSION_TAG(flag);
		break;
	}

	default:
	{
		fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_ID, TOKEN_INT_NUMBER, TOKEN_REAL_NUMBER] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
		recoveryFromError(EXPRESSION);
	}
	}
}

void parse_EXPRESSION_TAG(int flag)
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
	case TOKEN_DIV:
	{

		fprintf(yyoutSyn, "Rule (EXPRESSION_TAG -> * EXPRESSION | / EXPRESSION)\n");
		Token* peekNextToken = peek();
		if (peekNextToken)
		{
			if (strcmp(peekNextToken->lexeme, "0") == 0)
			{
				setError(DIVIDED_BY_ZERO, currentToken->lineNumber, "");
			}
		}

		parse_EXPRESSION(flag);
		break;
	}
	case TOKEN_MUL:
	{

		fprintf(yyoutSyn, "Rule (EXPRESSION_TAG -> * EXPRESSION | / EXPRESSION)\n");
		parse_EXPRESSION(flag);
		break;
	}

	case TOKEN_LEFT_SQUARE_BRACKET:
	{
		fprintf(yyoutSyn, "Rule (EXPRESSION_TAG -> VARIABLE_TAG)\n");
		printErrors(currentToken->lineNumber);
		back_token();
		parse_VARIABLE_TAG(EXPRESSION_TAG);
		break;
	}

	case TOKEN_SEMICOLON:
	{
		fprintf(yyoutSyn, "Rule (EXPRESSION_TAG -> ellipse)\n");
		if (tempEntry && tempEntry->subType == ARRAY)
		{
			setError(USING_ARRAY_WITHOUT_SQUARE_BRACKET, currentToken->lineNumber, tempEntry->name);
		}

		printErrors(currentToken->lineNumber);
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

void PrintAllVariableThatNeverUsed(SymTable* current_ptr)
{
	int i = 0;
	for (i = 0; i < HASH_ARRAY_SIZE; i++)
	{
		SymTableEntry *entry = current_ptr->HashingTable[i];
		while (entry)
		{
			if (entry->countInstance == 0)
			{
				fprintf(yyoutSem, "Warning #%s \t Line number:%3d\t Description: The variable '%s' has never been used\n", "W001",
					entry->defineInLineNumber, entry->name);
			}

			entry = entry->next;
		}
	}
}



