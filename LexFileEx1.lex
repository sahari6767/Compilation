%option noyywrap
%{
#include <stdio.h>
#include <stdlib.h>
#include "Tokens.h"
#include "Tokens.c"

int line=1;
%}

DIGIT    [0-9]
ALPHA [A-Za-z]
UNDERSCORE [_]
%%

"end" {create_and_store_token(TOKEN_KEYWORD_END, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_KEYWORD_END",line ,yytext);}

"real" {create_and_store_token(TOKEN_REAL_NUM, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_REAL_NUM",line ,yytext);}

"int" {create_and_store_token(TOKEN_INT, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_INT",line ,yytext);}

"." {create_and_store_token(TOKEN_DOT, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_DOT",line ,yytext);}

"block" {create_and_store_token(TOKEN_BLOCK, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_BLOCK",line ,yytext);}

"begin" {create_and_store_token(TOKEN_BEGIN, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_BEGIN",line ,yytext);}

"arr" {create_and_store_token(TOKEN_ARRAY, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_ARRAY",line ,yytext);}

"of" {create_and_store_token(TOKEN_OF, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_OF",line ,yytext);}

"type" {create_and_store_token(TOKEN_TYPE, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_TYPE",line ,yytext);}

"is" {create_and_store_token(TOKEN_IS, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_IS",line ,yytext);}

"enum" {create_and_store_token(TOKEN_ENUM, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_ENUM",line ,yytext);}

"struct" {create_and_store_token(TOKEN_STRUCT, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_STRUCT",line ,yytext);}

"switch" {create_and_store_token(TOKEN_SWITCH, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_SWITCH",line ,yytext);}

"default" {create_and_store_token(TOKEN_DEFAULT, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_DEFAULT",line ,yytext);}

"break" {create_and_store_token(TOKEN_BREAK, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_BREAK",line ,yytext);}

"case" {create_and_store_token(TOKEN_CASE, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_CASE",line ,yytext);}

{ALPHA}({ALPHA}|{DIGIT}|{UNDERSCORE}{ALPHA}|{UNDERSCORE}{DIGIT})*	{create_and_store_token(TOKEN_ID, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_ID",line ,yytext);}

"+" {create_and_store_token(TOKEN_AR_OP_ADD, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_AR_OP_ADD",line ,yytext);}

"-" {create_and_store_token(TOKEN_AR_OP_SUB, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_AR_OP_SUB",line ,yytext);}

"*" {create_and_store_token(TOKEN_AR_OP_MULTI, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_AR_OP_MULTI",line ,yytext);}

"/"  {create_and_store_token(TOKEN_AR_OP_DIVIDE, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_AR_OP_DIVIDE",line ,yytext);}

"=" {create_and_store_token(TOKEN_ASSIGNMENT, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_ASSIGNMENT",line ,yytext);}

"," {create_and_store_token(TOKEN_COMMA, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_COMMA",line ,yytext);}

":" {create_and_store_token(TOKEN_COLON, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_COLON",line ,yytext);}

";" {create_and_store_token(TOKEN_SEMICOLON, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_SEMICOLON",line ,yytext);}

"{" {create_and_store_token(TOKEN_LEFT_CURLY_BRACES, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_LEFT_CURLY_BRACES",line ,yytext);}

"[" {create_and_store_token(TOKEN_LEFT_BRACKETS, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_LEFT_BRACKETS",line ,yytext);}

"(" {create_and_store_token(TOKEN_LEFT_PARENTHESES, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_LEFT_PARENTHESES",line ,yytext);}

"}" {create_and_store_token(TOKEN_RIGHT_CURLY_BRACES, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_RIGHT_CURLY_BRACES",line ,yytext);}

"]" {create_and_store_token(TOKEN_RIGHT_BRACKETS, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_RIGHT_BRACKETS",line ,yytext);}

")" {create_and_store_token(TOKEN_RIGHT_PARENTHESES, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_RIGHT_PARENTHESES",line ,yytext);}

{UNDERSCORE}({ALPHA}|{DIGIT}|{UNDERSCORE}{ALPHA}|{UNDERSCORE}{DIGIT})* {create_and_store_token(TOKEN_TYPE_NAME, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_TYPE_NAME",line ,yytext);}

0|[1-9]{DIGIT}* {create_and_store_token(TOKEN_INTEGER, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_INTEGER",line ,yytext);}

0"."{DIGIT}+|[1-9]{DIGIT}*"."{DIGIT}+ {create_and_store_token(TOKEN_REAL, yytext, line);
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_REAL",line ,yytext);}

<<EOF>>		{create_and_store_token  (TOKEN_EOF , yytext , line);}
fprintf(yyout ,"Token - '{%s}' founded in line number: {%d}, lexeme: '{%s}'.\n", "TOKEN_EOF",line ,yytext);}


"\n"				{line++;}
" "|"\t" 			{}
	. 		 {fprintf(yyout ,"The character '{%s}' in line number: {%d} does not begin any legal token in the language\n",yytext,line);} 

%%

int main(){

yyin = fopen( "//home/osboxes//Documents//test1.txt", "r" );
yyout = fopen( "//home//ubuntu//Documents//test1_313173213_204159784_204367387_lex.txt", "w" );

yylex();
fclose(yyin);
fclose(yyout);
yyrestart(yyin);

yyin = fopen( "//home/osboxes//Documents//test2.txt", "r" );
yyout = fopen( "//home//ubuntu//Documents//test2_313173213_204159784_204367387_lex.txt", "w" );

yylex();
fclose(yyin);
fclose(yyout);
yyrestart(yyin);

return 0;
}


