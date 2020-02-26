%option noyywrap
%{
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include "Tokens.h"
#include "Parser.h"
#include "SymbolTable.h"
FILE *yyoutLex, *yyoutSyn,*yyoutSem;
void error(char*,int);
int line=1;
%}

DIGIT    [0-9]
ALPHA [A-Za-z]
UNDERSCORE [_]
%x comment
%%

"$$"   BEGIN(comment);
<comment>[^\n]                 
<comment>\n   {line++; BEGIN(0);}

"end" {return Handle_token(TOKEN_KEYWORD_END, yytext, line);}

"real" {return Handle_token(TOKEN_REAL, yytext, line);}

"integer" {return Handle_token(TOKEN_INTEGER, yytext, line);}

"." {return Handle_token(TOKEN_DOT, yytext, line);}

"block" {return Handle_token(TOKEN_BLOCK, yytext, line);}

"begin" {return Handle_token(TOKEN_BEGIN, yytext, line);}

"array" {return Handle_token(TOKEN_ARRAY, yytext, line);}

"of" {return Handle_token(TOKEN_OF, yytext, line);}

"type" {return Handle_token(TOKEN_TYPE, yytext, line);}

"is" {return Handle_token(TOKEN_IS, yytext, line);}

"enum" {return Handle_token(TOKEN_ENUM, yytext, line);}

"struct" {return Handle_token(TOKEN_STRUCT, yytext, line);}

"switch" {return Handle_token(TOKEN_SWITCH, yytext, line);}

"default" {return Handle_token(TOKEN_DEFAULT, yytext, line);}

"break" {return Handle_token(TOKEN_BREAK, yytext, line);}

"case" {return Handle_token(TOKEN_CASE, yytext, line);}

{ALPHA}({ALPHA}|{DIGIT}|{UNDERSCORE}{ALPHA}|{UNDERSCORE}{DIGIT})*	{return Handle_token(TOKEN_ID, yytext, line);}

"+" {return Handle_token(TOKEN_AR_OP_ADD, yytext, line);}

"-" {return Handle_token(TOKEN_AR_OP_SUB, yytext, line);}

"*" {return Handle_token(TOKEN_AR_OP_MULTI, yytext, line);}

"/"  {return Handle_token(TOKEN_AR_OP_DIVIDE, yytext, line);}

"=" {return Handle_token(TOKEN_ASSIGNMENT, yytext, line);}

"," {return Handle_token(TOKEN_COMMA, yytext, line);}

":" {return Handle_token(TOKEN_COLON, yytext, line);}

";" {return Handle_token(TOKEN_SEMICOLON, yytext, line);}

"{" {return Handle_token(TOKEN_LEFT_CURLY_BRACKETS, yytext, line);}

"[" {return Handle_token(TOKEN_LEFT_BRACKETS, yytext, line);}

"(" {return Handle_token(TOKEN_LEFT_PARENTHESES, yytext, line);}

"}" {return Handle_token(TOKEN_RIGHT_CURLY_BRACKETS, yytext, line);}

"]" {return Handle_token(TOKEN_RIGHT_BRACKETS, yytext, line);}

")" {return Handle_token(TOKEN_RIGHT_PARENTHESES, yytext, line);}

{UNDERSCORE}({ALPHA}|{DIGIT}|{UNDERSCORE}{ALPHA}|{UNDERSCORE}{DIGIT})* {return Handle_token(TOKEN_TYPE_NAME, yytext, line);}

[0-9]{DIGIT}* {return Handle_token(TOKEN_INTEGER_NUM, yytext, line);}

[0-9]{DIGIT}*"."{DIGIT}+ {return Handle_token(TOKEN_REAL_NUM, yytext, line);}

<<EOF>>		{return Handle_token(TOKEN_EOF , yytext , line);}

"\n"				{line++;}
" "
"\t"
"\v"
"\f"
"\r"			
	. 		 {error(yytext,line);} 

%%

void error(char *lexeme ,int lineNum)
{
	fprintf(yyoutLex, "\nThe character {%s} at line: {%d} does not begin any legal token in the language\n\n",lexeme, lineNum);
}

int main()
{
	extern int isFirstToken;
	char* pathToFileTest1 = "C:\\temp\\test1.txt";
	char* pathToFileTest2 = "C:\\temp\\test2.txt";
	char* pathToExportResultFileTestLex1 =  "C:\\temp\\test1_313173213_204159784_204367387_lex.txt";
	char* pathToExportResultFileTestLex2 =  "C:\\temp\\test2_313173213_204159784_204367387_lex.txt";
	char* pathToExportResultFileTestSyn1 =  "C:\\temp\\test1_313173213_204159784_204367387_syntactic.txt";
	char* pathToExportResultFileTestSyn2 =  "C:\\temp\\test2_313173213_204159784_204367387_syntactic.txt";
	char* pathToExportResultFileTestSem1 =  "C:\\temp\\test1_313173213_204159784_204367387_semantic.txt";
	char* pathToExportResultFileTestSem2 =  "C:\\temp\\test2_313173213_204159784_204367387_semantic.txt";
	eTOKENS kind;
	line = 1;
	
	printf("File Test1:\n");
	yyin = fopen(pathToFileTest1 ,"r");
	if(!yyin)
	{
		/* can't open file test1 for some reason */
		printf("Error opening file: C:\\temp\\test1.txt\n");
	}
	else
	{
		/* file is open */
		yyoutLex = fopen(pathToExportResultFileTestLex1 ,"w");
		yyoutSyn = fopen(pathToExportResultFileTestSyn1 ,"w");
		yyoutSem = fopen(pathToExportResultFileTestSem1 ,"w");
		mainParser();
		printf("Done! see result on C:\\temp\\test1_313173213_204159784_204367387_lex.txt\n");
		printf("Done! see result on C:\\temp\\test1_313173213_204159784_204367387_syntactic.txt\n");
		printf("Done! see result on C:\\temp\\test1_313173213_204159784_204367387_semantic.txt\n");
	}
	
	/* Close file */
	if(yyin)
		fclose (yyin);
	
	if(yyoutLex)
	{
		fclose (yyoutLex);
	}
	
	if(yyoutSyn)
	{
		fclose (yyoutSyn);
	}

	freeMemoryTokens(); /* Release memory allocation */
	line = 1; /* Reset the line number counter for the second file test  */
	isFirstToken = 1;
	
	printf("\n\n\nFile Test2:\n");
	yyin = fopen(pathToFileTest2 ,"r");
	if(!yyin)
	{
		/* can't open file test2 for some reason */
		printf("Error opening file: C:\\temp\\test2.txt\n");
	}
	else
	{
		/* file is open */
		yyoutLex = fopen(pathToExportResultFileTestLex2 ,"w");
		yyoutSyn = fopen(pathToExportResultFileTestSyn2 ,"w");
		yyoutSem = fopen(pathToExportResultFileTestSem2 ,"w");
		mainParser();
		printf("Done! see result on C:\\temp\\test2_313173213_204159784_204367387_lex.txt\n");
		printf("Done! see result on C:\\temp\\test2_313173213_204159784_204367387_syntactic.txt\n");
		printf("Done! see result on C:\\temp\\test2_313173213_204159784_204367387_semantic.txt\n");
	}

	/* Close file */
	if(yyin)
	{
		fclose (yyin);
	}
	
	if(yyoutLex)
	{
		fclose (yyoutLex);
	}
	
	if(yyoutSyn)
	{
		fclose (yyoutSyn);
	}

	if(yyoutSem)
	{
		fclose (yyoutSem);
	}

	freeMemoryTokens(); /* Release memory allocation */

	printf("\n\n\n\nPress any key to exit\n");
	_getch();
	return 0;
}


