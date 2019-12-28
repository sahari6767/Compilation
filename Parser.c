#include <stdio.h>
#include "Parser.h"

Token* nt;
extern FILE *yyin, *yyout;
extern char *eTokensString[];

void match(eTOKENS eType){
	nt = next_token();
	if(nt->kind != eType){
		fprintf(yyout, "Expected: token '{%u}' at line: %d,\nActual token: '{%u}', lexeme: '%s'",
				eType, nt->lineNumber, nt->kind, nt->lexeme);
	}
}

void Parse() {
	Parse_Program();
	match(TOK_EOF);
	PrepareToNextParse();
}

void Parse_Program(){
	fprintf(yyout, "PROGRAM -> BLOCK\n");
	Parse_Block();
}

void Parse_BLOCK(){
	eTOKENS Firsts[1] = { TOKEN_BLOCK };
	eTOKENS Follows[1] = { TOKEN_KEYWORD_END };
	int FirstsSize = 1;
	int FollowsSize = 1;

	nt = next_token();
	//// Note - erasing the switch ????
	switch(nt->kind)
	{
	case TOKEN_BLOCK:
		fprintf(yyout, "BLOCK -> block DECLERAITONS begin STATEMENTS end\n");
		match(TOKEN_BLOCK);
		Parse_Declarations();
		match(TOKEN_BEGIN);
		Parse_Statements();
		match(TOKEN_KEYWORD_END);
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 0);
		break;
	}
}

void Parse_Declarations(){
	eTOKENS Firsts[2] = { TOKEN_INT, TOKEN_REAL_NUM };
	eTOKENS Follows[1] = { TOKEN_SEMICOLON }; // Maybe need to add TOKEN_RIGHT_SEPERATOR to the array
	int FirstsSize = 2;
	int FollowsSize = 1;

	nt = next_token();
	switch(nt->kind){
	case TOKEN_INT:
		fprintf(yyout, "DECLARATIONS -> DECLARATION | DECLARATIONS; DECLARATIONS\n");
		nt = back_token();
		Parse_Declaration();
		Parse_Declarations_New();
		break;
	case TOKEN_REAL_NUM:
		fprintf(yyout, "DECLARATIONS -> DECLARATION | DECLARATIONS; DECLARATIONS\n");
		nt = back_token();
		Parse_Declaration();
		Parse_Declarations_New();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 0);
		break;
	}
}

void Parse_Declaration(){
	eTOKENS Firsts[2] = { TOKEN_INT, TOKEN_REAL_NUM };
	eTOKENS Follows[1] = { TOKEN_SEMICOLON };
	int FirstsSize = 2;
	int FollowsSize = 1;
	nt = next_token();

	switch(nt->kind){	
	case TOKEN_INT:
		fprintf(yyout, "DECLARATION -> VAR_DECLARATION | TYPE_DECLARATION\n");
		nt = back_token();
		Parse_Var_Declaration();
		Parse_Type_Declaration();
		break;
	case TOKEN_REAL_NUM:
		fprintf(yyout, "DECLARATION -> VAR_DECLARATION | TYPE_DECLARATION\n");
		nt = back_token();
		Parse_Var_Declaration();
		Parse_Type_Declaration();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 0);
		break;
	}
}

void Parse_Var_Declaration(){
	eTOKENS Firsts[2] = { TOKEN_ID, TOKEN_TYPE_NAME };
	eTOKENS Follows[4] = { TOKEN_COLON, TOKEN_SEMICOLON, TOKEN_ASSIGNMENT ,TOKEN_RIGHT_BRACKETS};
	int FirstsSize = 2;
	int FollowsSize = 4;
	nt = next_token();

	switch(nt->kind){	
	case TOKEN_ID:
		fprintf(yyout, "VAR_DECLARATION -> id Var_Declaration_New");
		Parse_Variable_New();
		break;
	case TOKEN_TYPE_NAME:
		fprintf(yyout, "VAR_DECLARATION -> id type_name");
		match(TOKEN_TYPE_NAME);
		break;	
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 0);
		break;
	}
}

void Parse_Var_Declaration_New(){
	eTOKENS Firsts[1] = { TOKEN_LEFT_BRACKETS };
	eTOKENS Follows[4] = { TOKEN_COLON, TOKEN_SEMICOLON, TOKEN_ASSIGNMENT ,TOKEN_RIGHT_BRACKETS};
	int FirstsSize = 1;
	int FollowsSize = 4;
	nt = next_token();

	switch(nt->kind){	
	case TOKEN_LEFT_BRACKETS:
		fprintf(yyout, "Var_Declaration_New -> [SIZE]\n");
		Parse_Size();		
		match(TOKEN_RIGHT_BRACKETS);
		break;
	case TOKEN_COLON:
		fprintf(yyout, "Var_Declaration_New -> epsilon\n");
		nt = back_token();
		break;
	case TOKEN_SEMICOLON:
		fprintf(yyout, "Var_Declaration_New -> epsilon\n");
		nt = back_token();
		break;
	case TOKEN_ASSIGNMENT:
		fprintf(yyout, "Var_Declaration_New -> epsilon\n");
		nt = back_token();
		break;
	case TOKEN_RIGHT_BRACKETS:
		fprintf(yyout, "Var_Declaration_New -> epsilon\n");
		nt = back_token();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 1);
		break;
	}
}

void Parse_Size(){
	eTOKENS Firsts[1] = { TOKEN_INTEGER };
	eTOKENS Follows[1] = { TOKEN_EOF };
	int FirstsSize = 1;
	int FollowsSize = 1;
	nt = next_token();

	switch(nt->kind){
	case TOKEN_INTEGER:
		fprintf(yyout, "SIZE -> int_num\n");
		match(TOKEN_INTEGER);
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 1);
		break;
}

void Parse_Simple_Type(){
	eTOKENS Firsts[2] = { TOKEN_INTEGER, TOKEN_REAL };
	eTOKENS Follows[2] = { TOKEN_ID, TOKEN_ARRAY };
	int FirstsSize = 2;
	int FollowsSize = 2;
	nt = next_token();
	switch(nt->kind){
	case TOKEN_INTEGER:
		fprintf(yyout, "The Type: integer\n");
		break;
	case TOKEN_REAL:
		fprintf(yyout, "The Type: real\n");
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 1);
		break;
	}
}
////////////////////////////////// ok /////////////////////////////////////

void Parse_Type_Declaration(){
	eTOKENS Firsts[3] = {  TOKEN_TYPE, TOKEN_TYPE_NAME, TOKEN_IS };
	eTOKENS Follows[1] = { TOKEN_RIGHT_CURLY_BRACES };/////////
	int FirstsSize = 3;
	int FollowsSize = 1;
	nt = next_token();

	switch(nt->kind){
	case TOKEN_TYPE:
		fprintf(yyout, "TYPE_DECLARTION -> type\n");
		nt = back_token();
		match(TOKEN_TYPE_NAME);
		nt = back_token();
		match(TOKEN_IS);
		Parse_Type_Indicator();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 1);
		break;
	}
}

void Parse_Type_Indicator(){
	eTOKENS Firsts[2] = {  TOKEN_ENUM, TOKEN_STRUCT };
	eTOKENS Follows[1] = { TOKEN_RIGHT_CURLY_BRACES };
	int FirstsSize = 2;
	int FollowsSize = 1;
	nt = next_token();

	switch(nt->kind){
	case TOKEN_ENUM:
		fprintf(yyout, "Type_Indicator -> ENUM_TYPE\n");
		Parse_Enum_Type();		
		break;
	case TOKEN_STRUCT:
		fprintf(yyout, "Type_Indicator -> TOKEN_STRUCT\n");
		Parse_Structure_Type();		
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 1);
		break;
	}
}

void Parse_Enum_Type(){
	eTOKENS Firsts[1] = { TOKEN_ENUM };
	eTOKENS Follows[1] = { ???? };////
	int FirstsSize = 1;
	int FollowsSize = 1;
	nt = next_token();

	switch(nt->kind){
	case TOKEN_ENUM:
		fprintf(yyout, "ENUM_TYPE -> enum {ID_LIST}\n");
		nt = back_token();
		Parse_Id_List();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 1);
		break;
	}
}

void Parse_Id_List(){
	eTOKENS Firsts[2] = { TOKEN_ID };
	eTOKENS Follows[4] = { TOKEN_COLON, TOKEN_SEMICOLON, TOKEN_ASSIGNMENT ,TOKEN_RIGHT_CURLY_BRACES};
	int FirstsSize = 1;
	int FollowsSize = 4;
	nt = next_token();

	switch(nt->kind){
	case TOKEN_ID:
		fprintf(yyout, "ID_LIST\n");
		nt = back_token();
		Parse_Id_List_New();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 1);
		break;
}

void Parse_Id_List_New(){
	eTOKENS Firsts[1] = { TOKEN_LEFT_CURLY_BRACES };
	eTOKENS Follows[4] = { TOKEN_COLON, TOKEN_SEMICOLON, TOKEN_ASSIGNMENT ,TOKEN_RIGHT_CURLY_BRACES};
	int FirstsSize = 1;
	int FollowsSize = 4;
	nt = next_token();

	switch(nt->kind){	
	case TOKEN_LEFT_CURLY_BRACES:
		fprintf(yyout, "id \n");	
		nt = back_token()
		match(TOKEN_RIGHT_CURLY_BRACES);
		break;
	case TOKEN_COLON:
		fprintf(yyout, "id -> epsilon\n");
		nt = back_token();
		break;
	case TOKEN_SEMICOLON:
		fprintf(yyout, "id -> epsilon\n");
		nt = back_token();
		break;
	case TOKEN_ASSIGNMENT:
		fprintf(yyout, "id -> epsilon\n");
		nt = back_token();
		break;
	case TOKEN_RIGHT_CURLY_BRACES:
		fprintf(yyout, "id -> epsilon\n");
		nt = back_token();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 1);
		break;
	}

}
void Parse_Structure_Type(){
	eTOKENS Firsts[1] = { TOKEN_STRUCT };
	eTOKENS Follows[1] = { TOKEN_RIGHT_CURLY_BRACES };
	int FirstsSize = 1;
	int FollowsSize = 1;
	nt = next_token();

	switch(nt->kind){
	case TOKEN_STRUCT:
		fprintf(yyout, "STRUCTUERE_TYPE -> struct { FIELDS }\n");
		nt = back_token();
		Parse_Fields();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 1);
		break;
	}
}
////////////////  We Stped Here(27.12.19) ////////////////
void Parse_Fields(){
	eTOKENS Firsts[2] = { TOKEN_INTEGER,TOKEN_REAL };
	eTOKENS Follows[2] = { TOKEN_SEMICOLON,TOKEN_RIGHT_BRACKETS };////
	int FirstsSize = 1;
	int FollowsSize = 2;
	nt = next_token();

	switch(nt->kind){
	case TOKEN_INTEGER:
		fprintf(yyout, "FIELDS -> FIELDS; FIELD | FIELD\n");
		nt = back_token();
		Parse_Fields_new();
		Parse_Field();
		break;
	case TOKEN_REAL:
		fprintf(yyout, "FIELDS -> FIELDS; FIELD | FIELD\n");
		nt = back_token();
		Parse_Fields_new();
		Parse_Field();
		break;

	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 2 );
		break;
	}
}

void Parse_Fields_new(){
	eTOKENS Firsts[1] = { TOKEN_SEMICOLON };
	eTOKENS Follows[2] = { TOKEN_SEMICOLON,TOKEN_RIGHT_CURLY_BRACES };////
	int FirstsSize = 1;
	int FollowsSize = 1;
	nt = next_token();

	switch(nt->kind){
	nt = next_token();
		if(nt->kind == TOKEN_INTEGER || nt->kind == TOKEN_REAL)
		{
			fprintf(yyout, "FIELDS -> FIELDS; FIELD | FIELD\n");
			nt = back_token();
			Parse_Fields();
		}
		else
		{
			fprintf(yyout, "Fields_new -> epsilon\n");
			nt = back_token();
			nt = back_token();
		}
		break;
	case TOKEN_RIGHT_CURLY_BRACES:
		nt = back_token();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 1);
		break;
	}
}

void Parse_Field(){
	eTOKENS Firsts[2] = { TOKEN_INTEGER,TOKEN_REAL };
	eTOKENS Follows[1] = { TOKEN_SEMICOLON };////
	int FirstsSize = 2;
	int FollowsSize = 1;
	nt = next_token();

	switch(nt->kind)
	{
	case TOKEN_INTEGER:
		fprintf(yyout, "FIELD -> VAR_ DECLARATION\n");
		nt = back_token();
		Parse_Var_Declaration();
		break;
	case TOKEN_REAL:
		fprintf(yyout, "FIELD -> VAR_ DECLARATION\n");
		nt = back_token();
		Parse_Var_Declaration();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 0 );
		break;
	}
}
////////////////  We Stped Here(28.12.19) ////////////////




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