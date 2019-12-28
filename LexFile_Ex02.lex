%option noyywrap
%{
#include <stdio.h>

#include "Token.h"
#include "Parser.h"

#define TESTS 2

void error(char*,int);

int lineNumber=-1;

%}

DIGIT		[0-9]
NONZERO		[1-9]
ALPHA		[a-zA-Z]
ALPHANUM	[a-zA-Z0-9]

ID				{ALPHA}("_"?{ALPHANUM})*
NUMBER_INT		({NONZERO}{DIGIT}*|0)
NUMBER_REAL	    ({NONZERO}{DIGIT}*|0)"."{DIGIT}+
COMMENT			"--".*
ENDLN			[\n]
WHITE			[ \t]
%%


"program" 		{return  Handle_token(TOK_KW_PROGRAM , yytext , lineNumber);}
"end" 			{return  Handle_token(TOK_KW_END , yytext , lineNumber);}
"real" 			{return  Handle_token(TOK_KW_REAL , yytext , lineNumber);}
"integer"		{return  Handle_token(TOK_KW_INTEGER , yytext , lineNumber);}
"void"			{return  Handle_token(TOK_KW_VOID , yytext , lineNumber);}
"return"		{return  Handle_token(TOK_KW_RETURN , yytext , lineNumber);}


";"				{return  Handle_token(TOK_SEP_SEMICOLON , yytext , lineNumber);}
","				{return  Handle_token(TOK_SEP_COMMA , yytext , lineNumber);}
"("				{return  Handle_token(TOK_SEP_L_BRCKT , yytext , lineNumber);}
")"				{return  Handle_token(TOK_SEP_R_BRCKT , yytext , lineNumber);}
"["				{return  Handle_token(TOK_SEP_L_SQBRCKT , yytext , lineNumber);}
"]"				{return  Handle_token(TOK_SEP_R_SQBRCKT , yytext , lineNumber);}
"{"				{return  Handle_token(TOK_SEP_L_CURBRCKT , yytext , lineNumber);}
"}"				{return  Handle_token(TOK_SEP_R_CURBRCKT , yytext , lineNumber);}


"*"				{return  Handle_token(TOK_OP_MULT , yytext , lineNumber);}
"/"				{return  Handle_token(TOK_OP_DIV , yytext , lineNumber);}
"="				{return  Handle_token(TOK_OP_ASSIGN , yytext , lineNumber);}


{ID}            {return  Handle_token(TOK_ID , yytext , lineNumber);}

{NUMBER_INT}           {return  Handle_token(TOK_NUMBER_INT , yytext , lineNumber);}
{NUMBER_REAL}          {return  Handle_token(TOK_NUMBER_REAL , yytext , lineNumber);}


{ENDLN}			{lineNumber++;}


{COMMENT}       {}
{WHITE}			{}

			
<<EOF>>			{return  Handle_token(TOK_EOF , yytext , lineNumber);}


.       		{error(yytext,lineNumber);}


%%


void error(char *lexeme ,int lineNum)
{
	printf("error: unrecognized character in line %4d: %4s\n",lineNum,lexeme);
}



int main(int argc, char *argv[])
{

	int test;
	int flagin,flagout;

	char filename[50]={0};
	
	eTOKENS kind;


	for(test = 1 ; test <= TESTS ; test++)
	{
		
		flagin = flagout = 0;

		/*generating input file name*/
		sprintf(filename,"%s%d%s","C:\\temp\\test",test,".txt");
		
		/*open input file*/
		if(!(yyin	=	fopen(filename,"r")))
		{ 
			printf("cannot open input file %s; please press any key to continue\n" ,filename);
			getchar();
			continue;
		}
		else
		{
			flagin = 1;
			lineNumber = 1;

			/*generating output file name*/
			sprintf(filename,"%s%d%s","C:\\temp\\out",test,"_312485436_308019918_lex.txt");

			/*open output file*/
			if(!(yyout	=	fopen(filename,"w")))
			{ 
				printf("cannot open output file %s; please press any key to continue\n" ,filename);
				getchar();
				continue;
			}
			else
			{
				printf("Lexical file for 'test%d.txt' has been created\n", test);
				flagout = 1;
			}
		}

		/*both files opened*/
		if(flagin*flagout)
		{
			/*looping yylex till EOF*/
			do{
				kind = yylex();
			} while(kind != TOK_EOF);
		}
		if(flagout)
			fclose(yyout);

		/* open 2*/ 
			/*generating output file name*/
			sprintf(filename,"%s%d%s","C:\\temp\\out",test,"_312485436_308019918_syntactic.txt");

			/*open output file*/
			if(!(yyout	=	fopen(filename,"w")))
			{ 
				printf("cannot open output file %s; please press any key to continue\n" ,filename);
				getchar();
				continue;
			}
			else
			{
				printf("Syntactic file for 'test%d.txt' has been created\n", test);
				flagout = 1;
			}
		/*both files opened*/
		if(flagin*flagout)
		{
			Parse();
		}

		/*close files*/
		if(flagin)
			fclose(yyin);
		if(flagout)
			fclose(yyout);
	}

	/****************************************************
	PAUSE TO SEE ERROR MSG: START
	*****************************************************/
	printf("\nPAUSE TO SEE ERROR MSG: Press any key to continue\n");
	getchar(); 
	
	/****************************************************
	PAUSE TO SEE ERROR MSG: END
	*****************************************************/
	return 0;
}
