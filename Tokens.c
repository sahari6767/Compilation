#include "Tokens.h"

/* This package describes the storage of tokens identified in the input text.
* The storage is a bi-directional list of nodes.
* Each node is an array of tokens; the size of this array is defined as TOKEN_ARRAY_SIZE.
* Such data structure supports an efficient way to manipulate tokens.

There are three functions providing an external access to the storage:
- function create_and_store_tokens ; it is called by the lexical analyzer when it identifies a legal token in the input text.
- functions next_token and back_token; they are called by parser during the syntax analysis (the second stage of compilation)
*/

int isFirstToken = 1;
int currentIndex = 0;
int currentIndexToken = 0;
int howMuchNodeAlocate = 0;
int isNextToken = 1;
extern int yylex();
extern Token* currentToken;
extern FILE *yyoutLex, *yyoutSyn;
Node* currentNode = NULL;
Node* headNode = NULL;
Node* currentNodeOfNextToken = NULL;

#define TOKEN_ARRAY_SIZE 100

/*
* This function creates a token and stores it in the storage.
*/
void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine)
{ 
	int length = strlen(lexeme)+1;
	
	// case 1: there is still no tokens in the storage.
	if (currentNode == NULL)
	{
		currentNode = (Node*)malloc(sizeof(Node));

		if(currentNode == NULL)
		{
			fprintf(yyout,"\nUnable to allocate memory! \n"); 
			exit(0);
		}
		currentNode->tokensArray = (Token*)calloc(sizeof(Token),TOKEN_ARRAY_SIZE);
		if(currentNode->tokensArray == NULL)
		{
			fprintf(yyout,"\nUnable to allocate memory! \n"); 
			exit(0);
		}
		currentNode->prev = NULL;
		currentNode->next = NULL;
	}

	// case 2: at least one token exsits in the storage.
	else
	{
		// the array (the current node) is full, need to allocate a new node
		if (currentIndex == TOKEN_ARRAY_SIZE - 1)
		{
			currentIndex = 0;
			currentNode->next = (Node*)malloc(sizeof(Node));

			if(currentNode == NULL)
			{
				fprintf(yyout,"\nUnable to allocate memory! \n"); 
				exit(0);
			}
			currentNode->next->prev = currentNode;
			currentNode = currentNode->next;
			currentNode->tokensArray = (Token*)calloc(sizeof(Token),TOKEN_ARRAY_SIZE);

			if(currentNode->tokensArray == NULL)
			{
				fprintf(yyout,"\nUnable to allocate memory! \n"); 
				exit(0);
			}
			currentNode->next = NULL;
		}

		// the array (the current node) is not full
		else
		{
			currentIndex++;
		}
	}

	currentNode->tokensArray[currentIndex].kind = kind;	
	currentNode->tokensArray[currentIndex].lineNumber = numOfLine;

	currentNode->tokensArray[currentIndex].lexeme = (char*)malloc(sizeof(char)*length);
	#ifdef _WIN32
		strcpy_s(currentNode->tokensArray[currentIndex].lexeme, length, lexeme);
	#else
		strcpy(currentNode->tokensArray[currentIndex].lexeme, lexeme);
	#endif		
}


char* convertFromTokenKindToString(eTOKENS i_Kind)
{
	switch(i_Kind)
	{
		case TOKEN_KEYWORD_END: return "TOKEN_KEYWORD_END";
		case TOKEN_REAL_NUM: return "TOKEN_REAL_NUM";
		case TOKEN_INT: return "TOKEN_INT";
		case TOKEN_DOT: return "TOKEN_DOT";
		case TOKEN_BLOCK: return "TOKEN_BLOCK";
		case TOKEN_BEGIN: return "TOKEN_BEGIN";
		case TOKEN_ARRAY: return "TOKEN_ARRAY";
		case TOKEN_OF: return "TOKEN_OF";
		case TOKEN_TYPE: return "TOKEN_TYPE";
		case TOKEN_IS: return "TOKEN_IS";
		case TOKEN_ENUM: return "TOKEN_ENUM";
		case TOKEN_STRUCT: return "TOKEN_STRUCT";
		case TOKEN_SWITCH: return "TOKEN_SWITCH";
		case TOKEN_DEFAULT: return "TOKEN_DEFAULT";
		case TOKEN_BREAK: return "TOKEN_BREAK";
		case TOKEN_CASE: return "TOKEN_CASE";
		case TOKEN_ID: return "TOKEN_ID";
		case TOKEN_AR_OP_ADD: return "TOKEN_AR_OP_ADD";
		case TOKEN_AR_OP_SUB: return "TOKEN_AR_OP_SUB";
		case TOKEN_AR_OP_MULTI: return "TOKEN_AR_OP_MULTI";
		case TOKEN_AR_OP_DIVIDE: return "TOKEN_AR_OP_DIVIDE";
		case TOKEN_ASSIGNMENT: return "TOKEN_ASSIGNMENT";
		case TOKEN_COMMA: return "TOKEN_COMMA";
		case TOKEN_COLON: return "TOKEN_COLON";
		case TOKEN_SEMICOLON: return "TOKEN_SEMICOLON";
		case TOKEN_LEFT_PARENTHESES: return "TOKEN_LEFT_PARENTHESES";
		case TOKEN_LEFT_BRACKETS: return "TOKEN_LEFT_BRACKETS";
		case TOKEN_LEFT_CURLY_BRACKETS: return "TOKEN_LEFT_CURLY_BRACKETS";
		case TOKEN_RIGHT_PARENTHESES: return "TOKEN_RIGHT_PARENTHESES";
		case TOKEN_RIGHT_BRACKETS: return "TOKEN_RIGHT_BRACKETS";
		case TOKEN_RIGHT_CURLY_BRACKETS: return "TOKEN_RIGHT_CURLY_BRACKETS";
		case TOKEN_TYPE_NAME: return "TOKEN_TYPE_NAME";
		case TOKEN_INTEGER: return "TOKEN_INTEGER";
		case TOKEN_REAL: return "TOKEN_REAL";
		case TOKEN_EOF: return "TOKEN_EOF";
	}
	return "Err";
}

/*
* This function returns the token in the storage that is stored immediately before the current token (if exsits).
*/
Token *back_token()
{	
	isNextToken--;
	if(currentIndexToken == 0)
	{
		// This means that you have to go back to prev Node 
		if(headNode == currentNode)
		{
			// Unable to go back from the firstNode
			return NULL;
		}
		
		currentNodeOfNextToken = currentNodeOfNextToken->prev;
		currentIndexToken = TOKEN_ARRAY_SIZE - 1;
	}
	else
	{
		// no need to back to prev Node, just decrement the index
		currentIndexToken--;
	}

	return  &currentNodeOfNextToken->tokensArray[currentIndexToken];
}
/*
* If the next token already exists in the storage (this happens when back_token was called before this call to next_token): 
*  this function returns the next stored token.
* Else: continues to read the input file in order to identify, create and store a new token (using yylex function);
*  returns the token that was created.
*/
Token *next_token() {
	Token *temp = NULL;
	if(isNextToken == 1)
	{
		yylex();
	}
	else
	{
		isNextToken++;
	}

	if(isFirstToken)
	{
		currentIndexToken = 0;
		isFirstToken = 0;
		currentToken = &currentNodeOfNextToken->tokensArray[currentIndexToken];
		return currentToken;
	}
	
	if(currentIndexToken == TOKEN_ARRAY_SIZE -1)
	{
		// That means we've reached the end of the Node
		if(currentNodeOfNextToken->next == NULL)
		{
			// no more node
			fprintf(yyoutSyn, "No more tokens! Done.");
			fprintf(yyoutSyn, "Parser finshed\n");
			exit(0);
		}
	
		currentNodeOfNextToken = currentNodeOfNextToken->next; // now the current node is the next one.
		currentIndexToken = 0; // reset the index on the array tokens.
	}
	else
	{
		// still in the current node.
		// just increment the index;
		currentIndexToken++;
	}

	temp = &currentNodeOfNextToken->tokensArray[currentIndexToken];
	return temp;
}

Token* peek()
{
	Token* peekToken = next_token();
	back_token();
	return peekToken;
}


void freeMemoryTokens()
{
	Node* toDeleteNode = headNode;
	int i = 0;
	int j = 0;

	while (toDeleteNode != NULL)
	{
		headNode = headNode->next; 
		if (howMuchNodeAlocate > 1)
		{
			// free entire full Node
			for (j = 0; j < TOKEN_ARRAY_SIZE - 1; j++)
				free(toDeleteNode->tokensArray[j].lexeme); // Delete alocation of lexma (char*)
		}
		else
		{   // for the last node: need to free only till the current index
			for (j = 0; j <= currentIndex; j++)
				free(toDeleteNode->tokensArray[j].lexeme); // free alocation of lexma (char*)
		}

		free(toDeleteNode->tokensArray); // free alocation of the array
		free(toDeleteNode); // free alocation of the node
		toDeleteNode = headNode;
		howMuchNodeAlocate--;
	}

	currentIndex = 0;
	toDeleteNode = headNode = currentNode = currentNodeOfNextToken = NULL;
}