#ifndef TOKENS_H
#define TOKENS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


extern FILE *yyin, *yyout;

typedef enum eTOKENS{
TOKEN_KEYWORD_END,
TOKEN_REAL_NUM,
TOKEN_INT,
TOKEN_DOT,
TOKEN_BLOCK,
TOKEN_BEGIN,
TOKEN_ARRAY,
TOKEN_OF,
TOKEN_TYPE,
TOKEN_IS,
TOKEN_ENUM,
TOKEN_STRUCT,
TOKEN_SWITCH,
TOKEN_DEFAULT,
TOKEN_BREAK,
TOKEN_CASE,
TOKEN_ID,
TOKEN_AR_OP_ADD,
TOKEN_AR_OP_SUB,
TOKEN_AR_OP_MULTI,
TOKEN_AR_OP_DIVIDE,
TOKEN_ASSIGNMENT,
TOKEN_COMMA,
TOKEN_COLON,
TOKEN_SEMICOLON,
TOKEN_LEFT_PARENTHESES,
TOKEN_LEFT_BRACKETS,
TOKEN_LEFT_CURLY_BRACES,
TOKEN_RIGHT_PARENTHESES,
TOKEN_RIGHT_BRACKETS,
TOKEN_RIGHT_CURLY_BRACES,
TOKEN_TYPE_NAME,
TOKEN_INTEGER,
TOKEN_REAL,
TOKEN_EOF 
} eTOKENS;

typedef struct Token{
	eTOKENS kind;
	char *lexeme;
	int lineNumber;
}Token;

typedef struct Node{
	Token *tokensArray;
	struct Node *prev;
	struct Node *next;
} Node;

void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine);
Token *next_token();
Token *back_token();

#endif
