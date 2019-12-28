#include <stdio.h>
#include "Parser.h"

Token* nt;
extern FILE *yyin, *yyout;
extern char *eTokensString[];

void match(eTOKENS eType)
{
	nt = next_token();
	if(nt->kind != eType)
	{
		fprintf(yyout, "Expected: token '{%u}' at line: %d,\nActual token: '{%u}', lexeme: '%s'",
				eType, nt->lineNumber, nt->kind, nt->lexeme);
	}
}


void Parse() {
	Parse_Program();
	match(TOK_EOF);
	PrepareToNextParse();
}

void Parse_Program()
{
	eTOKENS Firsts[1] = { TOK_KW_PROGRAM };
	eTOKENS Follows[1] = { TOK_EOF };
	int FirstsSize = 1;
	int FollowsSize = 1;

	nt = next_token();
	
	switch(nt->kind)
	{
	case TOK_KW_PROGRAM:
		fprintf(yyout, "PROGRAM -> program DEFINITIONS; STATEMENTS end FUNC_DEFINITIONS\n");
		Parse_Var_Definitions();
		match(TOK_SEP_SEMICOLON);
		Parse_Statements();
		match(TOK_KW_END);
		Parse_Func_Definitions();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 0);
		break;
	}
}

void Parse_Var_Definitions()
{
	eTOKENS Firsts[2] = { TOK_KW_INTEGER, TOK_KW_REAL };
	eTOKENS Follows[2] = { TOK_SEP_SEMICOLON, TOK_SEP_R_BRCKT };
	int FirstsSize = 2;
	int FollowsSize = 2;

	nt = next_token();
	switch(nt->kind)
	{
	case TOK_KW_INTEGER:
		fprintf(yyout, "VAR_DEFINITIONS -> VAR_DEFINITION VAR_DEFINITIONS_NEW\n");
		nt = back_token();
		Parse_Var_Definition();
		Parse_Var_Definitions_New();
		break;
	case TOK_KW_REAL:
		fprintf(yyout, "VAR_DEFINITIONS -> VAR_DEFINITION VAR_DEFINITIONS_NEW\n");
		nt = back_token();
		Parse_Var_Definition();
		Parse_Var_Definitions_New();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 0);
		break;
	}
}

void Parse_Var_Definitions_New()
{
	eTOKENS Firsts[1] = { TOK_SEP_SEMICOLON };
	eTOKENS Follows[2] = { TOK_SEP_SEMICOLON, TOK_SEP_R_BRCKT };
	int FirstsSize = 1;
	int FollowsSize = 2;

	nt = next_token();

	switch(nt->kind)
	{
	case TOK_SEP_SEMICOLON:
		nt = next_token();
		if(nt->kind == TOK_KW_INTEGER || nt->kind == TOK_KW_REAL)
		{
			fprintf(yyout, "VAR_DEFINITIONS_NEW -> ; VAR_DEFINITIONS\n");
			nt = back_token();
			Parse_Var_Definitions();
		}
		else
		{
			fprintf(yyout, "VAR_DEFINITIONS_NEW -> epsilon\n");
			nt = back_token();
			nt = back_token();
		}
		break;
	case TOK_SEP_R_BRCKT:
		nt = back_token();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 1);
		break;
	}
}

void Parse_Var_Definition()
{
	eTOKENS Firsts[2] = { TOK_KW_INTEGER, TOK_KW_REAL };
	eTOKENS Follows[1] = { TOK_SEP_SEMICOLON };
	int FirstsSize = 2;
	int FollowsSize = 1;
	nt = next_token();

	switch(nt->kind)
	{
	case TOK_KW_INTEGER:
		fprintf(yyout, "VAR_DEFINITION -> TYPE VARIABLES_LIST\n");
		nt = back_token();
		Parse_Type();
		Parse_Variables_List();
		break;
	case TOK_KW_REAL:
		fprintf(yyout, "VAR_DEFINITION -> TYPE VARIABLES_LIST\n");
		nt = back_token();
		Parse_Type();
		Parse_Variables_List();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 0);
		break;
	}
}

void Parse_Type()
{
	eTOKENS Firsts[2] = { TOK_KW_INTEGER, TOK_KW_REAL };
	eTOKENS Follows[1] = { TOK_ID };
	int FirstsSize = 2;
	int FollowsSize = 1;
	nt = next_token();

	switch(nt->kind)
	{
	case TOK_KW_INTEGER:
		fprintf(yyout, "TYPE -> integer\n");
		break;
	case TOK_KW_REAL:
		fprintf(yyout, "TYPE -> real\n");
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 0);
		break;
	}
}

void Parse_Variables_List()
{
	eTOKENS Firsts[1] = { TOK_ID };
	eTOKENS Follows[2] = { TOK_SEP_SEMICOLON, TOK_SEP_R_BRCKT };
	int FirstsSize = 1;
	int FollowsSize = 2;
	nt = next_token();

	switch(nt->kind)
	{
	case TOK_ID:
		fprintf(yyout, "VARIABLES_LIST -> VARIABLE VARIABLES_LIST_NEW\n");
		nt = back_token();
		Parse_Variable();
		Parse_Variables_List_New();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 0);
		break;
	}
}

void Parse_Variables_List_New()
{
	eTOKENS Firsts[1] = { TOK_SEP_COMMA };
	eTOKENS Follows[2] = { TOK_SEP_SEMICOLON, TOK_SEP_R_BRCKT };
	int FirstsSize = 1;
	int FollowsSize = 2;
	nt = next_token();

	switch(nt->kind)
	{
	case TOK_SEP_COMMA:
		fprintf(yyout, "VARIABLES_LIST_NEW -> , VARIABLE VARIABLES_LIST_NEW\n");
		Parse_Variable();
		Parse_Variables_List_New();
		break;
	case TOK_SEP_SEMICOLON:
		fprintf(yyout, "VARIABLES_LIST_NEW -> epsilon\n");
		nt = back_token();
		break;
	case TOK_SEP_R_BRCKT:
		fprintf(yyout, "VARIABLES_LIST_NEW -> epsilon\n");
		nt = back_token();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 1);
		break;
	}
}

void Parse_Variable()
{
	eTOKENS Firsts[1] = { TOK_ID };
	eTOKENS Follows[4] = { TOK_SEP_COMMA, TOK_SEP_SEMICOLON, TOK_OP_ASSIGN ,TOK_SEP_R_BRCKT};
	int FirstsSize = 1;
	int FollowsSize = 4;
	nt = next_token();

	switch(nt->kind)
	{
	case TOK_ID:
		fprintf(yyout, "VARIABLE -> id VARIABLE_NEW\n");
		Parse_Variable_New();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 0);
		break;
	}
}

void Parse_Variable_New()
{
	eTOKENS Firsts[1] = { TOK_SEP_L_SQBRCKT };
	eTOKENS Follows[4] = { TOK_SEP_COMMA, TOK_SEP_SEMICOLON, TOK_OP_ASSIGN,TOK_SEP_R_BRCKT };
	int FirstsSize = 1;
	int FollowsSize = 4;
	nt = next_token();

	switch(nt->kind)
	{
	case TOK_SEP_L_SQBRCKT:
		fprintf(yyout, "VARIABLE_NEW -> [int_number]\n");
		match(TOK_NUMBER_INT);
		match(TOK_SEP_R_SQBRCKT);
		break;
	case TOK_SEP_COMMA:
		fprintf(yyout, "VARIABLE_NEW -> epsilon\n");
		nt = back_token();
		break;
	case TOK_SEP_SEMICOLON:
		fprintf(yyout, "VARIABLE_NEW -> epsilon\n");
		nt = back_token();
		break;
	case TOK_OP_ASSIGN:
		fprintf(yyout, "VARIABLE_NEW -> epsilon\n");
		nt = back_token();
		break;
	case TOK_SEP_R_BRCKT:
		fprintf(yyout, "VARIABLE_NEW -> epsilon\n");
		nt = back_token();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 1);
		break;
	}
}

void Parse_Func_Definitions()
{
	eTOKENS Firsts[3] = { TOK_KW_VOID, TOK_KW_REAL, TOK_KW_INTEGER};
	eTOKENS Follows[1] = { TOK_EOF };
	int FirstsSize = 3;
	int FollowsSize = 1;
	nt = next_token();

	switch(nt->kind)
	{
	case TOK_KW_VOID:
		fprintf(yyout, "FUNC_DEFINITIONS -> FUNC_DEFINITION FUNC_DEFINITIONS_NEW\n");
		nt = back_token();
		Parse_Func_Definition();
		Parse_Func_Definitions_New();
		break;
	case TOK_KW_INTEGER:
		fprintf(yyout, "FUNC_DEFINITIONS -> FUNC_DEFINITION FUNC_DEFINITIONS_NEW\n");
		nt = back_token();
		Parse_Func_Definition();
		Parse_Func_Definitions_New();
		break;
	case TOK_KW_REAL:
		fprintf(yyout, "FUNC_DEFINITIONS -> FUNC_DEFINITION FUNC_DEFINITIONS_NEW\n");
		nt = back_token();
		Parse_Func_Definition();
		Parse_Func_Definitions_New();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 0);
		break;
	}
}

void Parse_Func_Definitions_New()
{
	eTOKENS Firsts[3] = { TOK_KW_VOID, TOK_KW_REAL, TOK_KW_INTEGER};
	eTOKENS Follows[1] = { TOK_EOF };
	int FirstsSize = 3;
	int FollowsSize = 1;
	nt = next_token();

	switch(nt->kind)
	{
	case TOK_KW_VOID:
		fprintf(yyout, "FUNC_DEFINITIONS_NEW -> FUNC_DEFINITION FUNC_DEFINITIONS_NEW\n");
		nt = back_token();
		Parse_Func_Definition();
		Parse_Func_Definitions_New();
		break;
	case TOK_KW_INTEGER:
		fprintf(yyout, "FUNC_DEFINITIONS_NEW -> FUNC_DEFINITION FUNC_DEFINITIONS_NEW\n");
		nt = back_token();
		Parse_Func_Definition();
		Parse_Func_Definitions_New();
		break;
	case TOK_KW_REAL:
		fprintf(yyout, "FUNC_DEFINITIONS_NEW -> FUNC_DEFINITION FUNC_DEFINITIONS_NEW\n");
		nt = back_token();
		Parse_Func_Definition();
		Parse_Func_Definitions_New();
		break;
	case TOK_EOF:
		fprintf(yyout, "FUNC_DEFINITIONS_NEW -> epsilon\n");
		nt = back_token();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 1);
		break;
	}
}


void Parse_Func_Definition()
{
	eTOKENS Firsts[3] = { TOK_KW_VOID, TOK_KW_REAL, TOK_KW_INTEGER};
	eTOKENS Follows[3] = { TOK_KW_VOID, TOK_KW_REAL, TOK_KW_INTEGER};
	int FirstsSize = 3;
	int FollowsSize = 3;
	nt = next_token();

	switch(nt->kind)
	{
	case TOK_KW_VOID:
		fprintf(yyout, "FUNC_DEFINITION -> RETURNED_TYPE id(PARAM_DEFINITIONS) BLOCK\n");
		nt = back_token();
		Parse_Returned_Type();
		match(TOK_ID);
		match(TOK_SEP_L_BRCKT);
		Parse_Param_Definitions();
		match(TOK_SEP_R_BRCKT);
		Parse_Block();
		break;
	case TOK_KW_INTEGER:
		fprintf(yyout, "FUNC_DEFINITION -> RETURNED_TYPE id(PARAM_DEFINITIONS) BLOCK\n");
		nt = back_token();
		Parse_Returned_Type();
		match(TOK_ID);
		match(TOK_SEP_L_BRCKT);
		Parse_Param_Definitions();
		match(TOK_SEP_R_BRCKT);
		Parse_Block();
		break;
	case TOK_KW_REAL:
		fprintf(yyout, "FUNC_DEFINITION -> RETURNED_TYPE id(PARAM_DEFINITIONS) BLOCK\n");
		nt = back_token();
		Parse_Returned_Type();
		match(TOK_ID);
		match(TOK_SEP_L_BRCKT);
		Parse_Param_Definitions();
		match(TOK_SEP_R_BRCKT);
		Parse_Block();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 0);
		break;
	}
}

void Parse_Returned_Type()
{
	eTOKENS Firsts[3] = { TOK_KW_VOID, TOK_KW_REAL, TOK_KW_INTEGER};
	eTOKENS Follows[1] = { TOK_ID };
	int FirstsSize = 3;
	int FollowsSize = 1;
	nt = next_token();

	switch(nt->kind)
	{
	case TOK_KW_VOID:
		fprintf(yyout, "RETURNED_TYPE -> void\n");
		break;
	case TOK_KW_REAL:
		fprintf(yyout, "RETURNED_TYPE -> TYPE\n");
		nt = back_token();
		Parse_Type();
		break;
	case TOK_KW_INTEGER:
		fprintf(yyout, "RETURNED_TYPE -> TYPE\n");
		nt = back_token();
		Parse_Type();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 0);
		break;
	}
}

void Parse_Param_Definitions()
{
	eTOKENS Firsts[2] = { TOK_KW_REAL, TOK_KW_INTEGER};
	eTOKENS Follows[1] = { TOK_SEP_R_BRCKT };
	int FirstsSize = 2;
	int FollowsSize = 1;
	nt = next_token();

	switch(nt->kind)
	{
	case TOK_KW_REAL:
		fprintf(yyout, "PARAM_DEFINITIONS -> VAR_DEFINITIONS\n");
		nt = back_token();
		Parse_Var_Definitions();
		break;
	case TOK_KW_INTEGER:
		fprintf(yyout, "PARAM_DEFINITIONS -> VAR_DEFINITIONS\n");
		nt = back_token();
		Parse_Var_Definitions();
		break;
	case TOK_SEP_R_BRCKT:
		fprintf(yyout, "PARAM_DEFINITIONS -> epsilon\n");
		nt = back_token();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 1);
		break;
	}
}

void Parse_Statements()
{
	eTOKENS Firsts[3] = { TOK_ID, TOK_SEP_L_CURBRCKT, TOK_KW_RETURN };
	eTOKENS Follows[2] = { TOK_SEP_R_CURBRCKT, TOK_KW_END };
	int FirstsSize = 3;
	int FollowsSize = 2;
	nt = next_token();

	switch(nt->kind)
	{
	case TOK_ID:
		fprintf(yyout, "STATEMENTS -> STATEMENT ; STATEMENTS_NEW\n");
		nt = back_token();
		Parse_Statement();
		match(TOK_SEP_SEMICOLON);
		Parse_Statements_New();
		break;
	case TOK_SEP_L_CURBRCKT:
		fprintf(yyout, "STATEMENTS -> STATEMENT ; STATEMENTS_NEW\n");
		nt = back_token();
		Parse_Statement();
		match(TOK_SEP_SEMICOLON);
		Parse_Statements_New();
		break;
	case TOK_KW_RETURN:
		fprintf(yyout, "STATEMENTS -> STATEMENT ; STATEMENTS_NEW\n");
		nt = back_token();
		Parse_Statement();
		match(TOK_SEP_SEMICOLON);
		Parse_Statements_New();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 0);
		break;
	}
}

void Parse_Statements_New()
{
	eTOKENS Firsts[3] = { TOK_ID, TOK_SEP_L_CURBRCKT, TOK_KW_RETURN };
	eTOKENS Follows[2] = { TOK_SEP_R_CURBRCKT, TOK_KW_END };
	int FirstsSize = 3;
	int FollowsSize = 2;
	nt = next_token();

	switch(nt->kind)
	{
	case TOK_ID:
		fprintf(yyout, "STATEMENTS_NEW -> STATEMENTS\n");
		nt = back_token();
		Parse_Statements();
		break;
	case TOK_SEP_L_CURBRCKT:
		fprintf(yyout, "STATEMENTS_NEW -> STATEMENTS\n");
		nt = back_token();
		Parse_Statements();
		break;
	case TOK_KW_RETURN:
		fprintf(yyout, "STATEMENTS_NEW -> STATEMENTS\n");
		nt = back_token();
		Parse_Statements();
		break;
	case TOK_SEP_R_CURBRCKT:
		fprintf(yyout, "STATEMENTS_NEW -> epsilon\n");
		nt = back_token();
		break;
	case TOK_KW_END:
		fprintf(yyout, "STATEMENTS_NEW -> epsilon\n");
		nt = back_token();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 1);
		break;
	}
}

void Parse_Statement()
{
	eTOKENS Firsts[3] = { TOK_ID, TOK_SEP_L_CURBRCKT, TOK_KW_RETURN };
	eTOKENS Follows[1] = { TOK_SEP_SEMICOLON };
	int FirstsSize = 3;
	int FollowsSize = 1;
	nt = next_token();

	switch(nt->kind)
	{
		case TOK_ID:
		fprintf(yyout, "STATEMENT -> id STATEMENT_NEW2\n");
		Parse_Statement_New2();
		break;
	case TOK_SEP_L_CURBRCKT:
		fprintf(yyout, "STATEMENT -> BLOCK\n");
		nt = back_token();
		Parse_Block();
		break;
	case TOK_KW_RETURN:
		fprintf(yyout, "STATEMENT -> return STATEMENT_NEW\n");
		Parse_Statement_New();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 0);
		break;
	}
}

void Parse_Statement_New()
{
	eTOKENS Firsts[3] = { TOK_NUMBER_INT, TOK_NUMBER_REAL, TOK_ID };
	eTOKENS Follows[1] = { TOK_SEP_SEMICOLON };
	int FirstsSize = 3;
	int FollowsSize = 1;
	nt = next_token();

	switch(nt->kind)
	{
	case TOK_NUMBER_INT:
		fprintf(yyout, "STATEMENT_NEW -> EXPRESSION\n");
		nt = back_token();
		Parse_Expression();
		break;
	case TOK_NUMBER_REAL:
		fprintf(yyout, "STATEMENT_NEW -> EXPRESSION\n");
		nt = back_token();
		Parse_Expression();
		break;
	case TOK_ID:
		fprintf(yyout, "STATEMENT_NEW -> EXPRESSION\n");
		nt = back_token();
		Parse_Expression();
		break;
	case TOK_SEP_SEMICOLON:
		fprintf(yyout, "STATEMENT_NEW -> epsilon\n");
		nt = back_token();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 1);
		break;
	}
}

void Parse_Statement_New2()
{
	eTOKENS Firsts[3] = { TOK_SEP_L_SQBRCKT, TOK_SEP_L_BRCKT, TOK_OP_ASSIGN };
	eTOKENS Follows[1] = { TOK_SEP_SEMICOLON };
	int FirstsSize = 3;
	int FollowsSize = 1;
	nt = next_token();

	switch(nt->kind)
	{
	case TOK_SEP_L_SQBRCKT:
		fprintf(yyout, "STATEMENT_NEW2 -> VARIABLE_NEW = EXPRESSION\n");
		nt = back_token();
		Parse_Variable_New();
		match(TOK_OP_ASSIGN);
		Parse_Expression();
		break;
	case TOK_SEP_L_BRCKT:
		fprintf(yyout, "STATEMENT_NEW -> (FUNCTION_CALL_NEW)\n");
		Parse_Function_Call_New();
		match(TOK_SEP_R_BRCKT);
		break;
	case TOK_OP_ASSIGN:
		fprintf(yyout, "STATEMENT_NEW -> VARIABLE_NEW = EXPRESSION\n");
		Parse_Expression();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 0);
		break;
	}
}


void Parse_Function_Call_New()
{
	eTOKENS Firsts[1] = { TOK_ID };
	eTOKENS Follows[1] = { TOK_SEP_R_BRCKT };
	int FirstsSize = 1;
	int FollowsSize = 1;
	nt = next_token();

	switch(nt->kind)
	{
	case TOK_ID:
		fprintf(yyout, "FUNCTION_CALL_NEW -> VARIABLES_LIST\n");
		nt = back_token();
		Parse_Variables_List();
		break;
	case TOK_SEP_R_BRCKT:
		fprintf(yyout, "FUNCTION_CALL_NEW -> epsilon\n");
		nt = back_token();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 1);
		break;
	}	
}

void Parse_Block()
{
	eTOKENS Firsts[1] = { TOK_SEP_L_CURBRCKT };
	eTOKENS Follows[5] = { TOK_KW_VOID, TOK_KW_REAL, TOK_KW_INTEGER, TOK_SEP_SEMICOLON, TOK_EOF };
	int FirstsSize = 1;
	int FollowsSize = 5;
	nt = next_token();

	switch(nt->kind)
	{
	case TOK_SEP_L_CURBRCKT:
		fprintf(yyout, "BLOCK -> { VAR_DEFINITIONS; STATEMENTS }\n");
		Parse_Var_Definitions();
		match(TOK_SEP_SEMICOLON);
		Parse_Statements();
		match(TOK_SEP_R_CURBRCKT);
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 0);
		break;
	}
}


void Parse_Expression()
{
	eTOKENS Firsts[3] = { TOK_NUMBER_INT, TOK_NUMBER_REAL, TOK_ID };
	eTOKENS Follows[1] = { TOK_SEP_SEMICOLON };
	int FirstsSize = 3;
	int FollowsSize = 1;
	nt = next_token();

	switch(nt->kind)
	{
	case TOK_ID:
		fprintf(yyout, "EXPRESSION -> id EXPRESSION_NEW\n");
		Parse_Expression_New();
		break;
	case TOK_NUMBER_INT:
		fprintf(yyout, "EXPRESSION -> int_number\n");
		break;
	case TOK_NUMBER_REAL:
		fprintf(yyout, "EXPRESSION -> real_number\n");
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 0);
		break;
	}
}

void Parse_Expression_New()
{
	eTOKENS Firsts[3] = { TOK_SEP_L_SQBRCKT, TOK_OP_MULT, TOK_OP_DIV };
	eTOKENS Follows[1] = { TOK_SEP_SEMICOLON };
	int FirstsSize = 3;
	int FollowsSize = 1;
	nt = next_token();

	switch(nt->kind)
	{
	case TOK_SEP_L_SQBRCKT:
		fprintf(yyout, "EXPRESSION_NEW -> VARIABLE_NEW\n");
		nt = back_token();
		Parse_Variable_New();
		break;
	case TOK_OP_MULT:
		fprintf(yyout, "EXPRESSION_NEW -> ar_op EXPRESSION\n");
		Parse_Expression();
		break;
	case TOK_OP_DIV:
		fprintf(yyout, "EXPRESSION_NEW -> ar_op EXPRESSION\n");
		Parse_Expression();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 1);
		break;
	}
}

/*		Error handling		*/

void Error_Handle(eTOKENS *Firsts, eTOKENS *Follows, int FirstsSize, int FollowsSize, int IsNullable) {
	Error_Output(nt, Firsts, FirstsSize);
	Following_Token(Follows, FollowsSize);
}

void Following_Token(eTOKENS* Follows, int size) {
	while (!Is_Token_In_Follows(nt, Follows, size) && nt->kind != EOF) {
		nt = next_token();
	}
	if (Is_Token_In_Follows(nt, Follows, size)) {
		nt = back_token();
	}
	else {
		exit(0);
	}
}

void Error_Output(Token* token, eTOKENS* Follows, int size) {
	fprintf(yyout, "Expected one of tokens: ");
	Print_Follows(Follows, size);
	fprintf(yyout, " at line: '%d',\nActual token: '{%s}', lexeme: '{%s}'.\n", token->lineNumber, eTokensString[token->kind], token->lexeme);
}

void Print_Follows(eTOKENS* Follows, int size) {
	int i;
	for (i = 0; i < size; i++) {
		fprintf(yyout, "  {%s}  ", eTokensString[Follows[i]]);
	}
}

/*  	follow(X) function		*/

int Is_Token_In_Follows(Token* token, eTOKENS* Follows, int size) {
	int i;
	int Token_In_Follows = 0;

	for (i = 0; i < size; i++) {
		if (token->kind == Follows[i]) {
			Token_In_Follows = 1;
			break;
		}
	}
	return Token_In_Follows;
}