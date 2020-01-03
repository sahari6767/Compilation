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
	case TOKEN_COLON://Need to put colon(match)
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

void Parse_Statements()
{
	eTOKENS Firsts[4] = { TOKEN_ID, TOKEN_SWITCH, TOKEN_BREAK, TOKEN_BLOCK };
	eTOKENS Follows[2] = { TOKEN_RIGHT_CURLY_BRACES, TOKEN_KEYWORD_END };
	////////////////  We Stped Here(28.12.19) ////////////////
	int FirstsSize = 3;
	int FollowsSize = 2;
	nt = next_token();

	switch(nt->kind)
	{
	case TOKEN_ID:
		fprintf(yyout, "STATEMENTS -> STATEMENT ; STATEMENTS_NEW\n");
		nt = back_token();
		Parse_Statement();
		Parse_Statements_New();
		break;
	case TOKEN_SWITCH:
		fprintf(yyout, "STATEMENTS -> STATEMENT ; STATEMENTS_NEW\n");
		nt = back_token();
		Parse_Statement();
		Parse_Statements_New();
		break;
	case TOKEN_BREAK:
		fprintf(yyout, "STATEMENTS -> STATEMENT ; STATEMENTS_NEW\n");
		nt = back_token();
		Parse_Statement();
		Parse_Statements_New();
		break;
	case TOKEN_BLOCK:
		fprintf(yyout, "STATEMENTS -> STATEMENT ; STATEMENTS_NEW\n");
		nt = back_token();
		Parse_Statement();
		Parse_Statements_New();
		break;
	default: 
		Error_Handle(Firsts, Follows, FirstsSize, FollowsSize, 0);
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