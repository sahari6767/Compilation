#include "Tokens.h"
#include "Parser.h"
#define _CRT_SECURE_NO_WARNINGS

extern FILE *yyoutLex, *yyoutSyn;
extern Node* currentNode;
extern Node* headNode;
Token *currentToken;

void match(eTOKENS i_ExpectedTokenKind)
{
	currentToken = next_token();
	if (currentToken->kind != i_ExpectedTokenKind)
	{
		fprintf(yyoutSyn, "Expected token of type {%s} at line: {%d}, Actual token of type {%s}, lexeme: {%s}\n", convertFromTokenKindToString(i_ExpectedTokenKind), currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
	}
}

void mainParser()
{
	fprintf(yyoutSyn, "Parser starting\n");
	parse_PROGRAM();
	match(TOKEN_EOF);
	fprintf(yyoutSyn, "Parser finshed\n");
}

void parse_PROGRAM()
{
	fprintf(yyoutSyn, "Rule (PROGRAM -> BLOCK)\n");
	parse_BLOCK();
}

void parse_BLOCK()
{
	currentToken = next_token();
	switch(currentToken->kind)
	{
		case TOKEN_BLOCK:
		{
			fprintf(yyoutSyn, "Rule (BLOCK -> block DECLARATIONS begin STATEMENTS end)\n");
			parse_DECLARATIONS();
			match(TOKEN_BEGIN);
			parse_STATEMENTS();
			match(TOKEN_KEYWORD_END);
			break;
		}
		
		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_BLOCK] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(BLOCK);
		}
	}
}

void parse_DECLARATIONS()
{
	fprintf(yyoutSyn, "Rule (DECLARATIONS -> DECLARATION DECLARATIONS_NEW)\n");
	parse_DECLARATION();
	parse_DECLARATIONS_NEW();

}

void parse_DECLARATIONS_NEW()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_SEMICOLON:
		{
				fprintf(yyoutSyn, "Rule (DECLARATIONS_NEW -> ; DECLARATIONS)\n");
				parse_DECLARATIONS();
				break;
		}

		case TOKEN_BEGIN:
		{
			fprintf(yyoutSyn, "Rule (DECLARATIONS_NEW -> epsilon)\n");
			back_token();
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_SEMICOLON, TOKEN_RIGHT_PARENTHESES] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(DECLARATIONS_NEW);
		}
	}
}

void parse_DECLARATION()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_ID:
		{
			fprintf(yyoutSyn, "Rule (DECLARATION -> VAR_DECLARATION)\n");
			back_token();
			parse_VAR_DECLARATION();
			break;
		}
		
		case TOKEN_TYPE:
		{
			fprintf(yyoutSyn, "Rule (DECLARATION -> TYPE_DECLARATION)\n");
			back_token();
			parse_TYPE_DECLARATION();
			break;
		}
/*
		case TOKEN_BEGIN: // we added that  for test2
		{
			currentToken = back_token();
			if (currentToken->kind != TOKEN_BLOCK)
			{
				fprintf(yyoutSyn, "Rule (DECLARATION -> epsilon)\n");
			}
			else
			{
				fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_ID,TOKEN_TYPE] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			}
			break;
		}*/

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_ID,TOKEN_TYPE] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(DECLARATION);
		}
	}

}

void parse_VAR_DECLARATION()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_ID:
		{
			fprintf(yyoutSyn, "Rule (VAR_DECLARATION -> id : VAR_DECLARATION_NEW)\n");
			match(TOKEN_COLON);
			parse_VAR_DECLARATION_NEW();
			break;
		}
		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_ID] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(VAR_DECLARATION);
		}
	}
}

void parse_VAR_DECLARATION_NEW()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_INTEGER:
		{
			fprintf(yyoutSyn, "Rule (VAR_DECLARATION_NEW -> SIMPLE_TYPE)\n"); 
			back_token();
			parse_SIMPLE_TYPE();
			break;
		}
	
		case TOKEN_REAL:
		{
			fprintf(yyoutSyn, "Rule (VAR_DECLARATION_NEW -> SIMPLE_TYPE)\n"); 
			back_token();
			parse_SIMPLE_TYPE();
			break;
		}
	
		case TOKEN_ARRAY:
		{
			fprintf(yyoutSyn, "VAR_DECLARATION_NEW -> array [SIZE] of SIMPLE_TYPE)\n"); 
			match(TOKEN_LEFT_BRACKETS);
			parse_SIZE();
			match(TOKEN_RIGHT_BRACKETS);
			match(TOKEN_OF);
			parse_SIMPLE_TYPE();
			break;
		}
	
		case TOKEN_TYPE_NAME:
		{
			fprintf(yyoutSyn, "VAR_DECLARATION_NEW -> type_name)\n"); 
			break;
		}
		
		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_INTEGER,TOKEN_REAL,TOKEN_ARRAY,TOKEN_TYPE_NAME] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(VAR_DECLARATION_NEW);
		}
	}
}

void parse_SIZE()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_INTEGER_NUM:
		{
			fprintf(yyoutSyn, "SIZE -> int_num)\n");
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_INTEGER_NUM] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(SIZE);

		} 		
	}
}

void parse_SIMPLE_TYPE()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_INTEGER:
		{
			fprintf(yyoutSyn, "Rule (SIMPLE_TYPE -> integer\n");
			break;
		}
		case TOKEN_REAL:
		{
			fprintf(yyoutSyn, "Rule (SIMPLE_TYPE -> real\n"); 
			break;
		}
		default:
		{
		fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_INTEGER,TOKEN_REAL] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
		recoveryFromError(SIMPLE_TYPE);
		}
	}
}

void parse_TYPE_DECLARATION()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_TYPE:
		{
			fprintf(yyoutSyn, "Rule (TYPE_DECLARATION -> type type_name is TYPE_INDICATOR\n");
			match(TOKEN_TYPE_NAME);
			match(TOKEN_IS);
			parse_TYPE_INDICATOR();
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_TYPE] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(TYPE_DECLARATION);
		}
	}

}

void parse_TYPE_INDICATOR()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_ENUM:
		{
			fprintf(yyoutSyn, "Rule (TYPE_INDICATOR -> ENUM_TYPE\n");
			back_token();
			parse_ENUM_TYPE();
			break;
		}
		case TOKEN_STRUCT:
		{
			fprintf(yyoutSyn, "Rule (TYPE_INDICATOR -> STRUCTURE_TYPE\n");
			back_token();
			parse_STRUCTURE_TYPE();
			break;
		}
		
		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_ENUM,TOKEN_STRUCT] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(TYPE_INDICATOR);

		}
	}
}

void parse_ENUM_TYPE()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_ENUM:
		{
			fprintf(yyoutSyn, "Rule (ENUM_TYPE -> enum { ID_LIST }\n");
			match(TOKEN_LEFT_CURLY_BRACKETS);
			parse_ID_LIST();
			match(TOKEN_RIGHT_CURLY_BRACKETS);
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_ENUM] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(ENUM_TYPE);

		}
	}
}

void parse_ID_LIST()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_ID:
		{
			fprintf(yyoutSyn, "Rule (ID_LIST -> id ID_LIST_NEW\n");
			parse_ID_LIST_NEW();
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_ID] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(ID_LIST);
		}
	}
}

void parse_ID_LIST_NEW()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_COMMA:
		{
			fprintf(yyoutSyn, "ID_LIST_NEW -> , ID_LIST_NEW\n");
			parse_ID_LIST();
			break;
		}

		case TOKEN_RIGHT_CURLY_BRACKETS:
		{
			fprintf(yyoutSyn, "ID_LIST_NEW -> epsilon\n");
			back_token();
			break;			
		}
		
		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_COMMA,TOKEN_RIGHT_CURLY_BRACKETS] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(ID_LIST_NEW);
		}
	}
}

void parse_STRUCTURE_TYPE()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_STRUCT:
		{
			fprintf(yyoutSyn, "Rule (STRUCTURE_TYPE -> struct { FIELDS }\n");
			match(TOKEN_LEFT_CURLY_BRACKETS);
			parse_FIELDS();
			match(TOKEN_RIGHT_CURLY_BRACKETS);
			break;
		}
		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_STRUCT] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(STRUCTURE_TYPE);
		}
	}
}

void parse_FIELDS()
{
	fprintf(yyoutSyn, "Rule (FIELDS -> FIELD FIELD_NEW\n");
	parse_FIELD();
	parse_FIELDS_NEW();
}

void parse_FIELD()
{
	fprintf(yyoutSyn, "Rule (FIELD -> VAR_DECLARATION\n");
	parse_VAR_DECLARATION();
}

void parse_FIELDS_NEW()
{
	currentToken = next_token();	
	switch (currentToken->kind)
	{
		case TOKEN_SEMICOLON:
		{
			fprintf(yyoutSyn, "Rule (FIELDS_NEW -> ; FIELD FIELDS_NEW\n");
			parse_FIELD();
			parse_FIELDS_NEW();
			break;	
		}
		case TOKEN_RIGHT_CURLY_BRACKETS:
		{
			fprintf(yyoutSyn, "Rule (FIELDS_NEW -> ; epsilon\n");
			back_token();
			break;
		}
		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_SEMICOLON, TOKEN_RIGHT_CIRCLE_BRACKET] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(FIELDS_NEW);
		}
	}
}

void parse_STATEMENTS()
{
	fprintf(yyoutSyn, "Rule (STATEMENTS -> STATEMENT STATEMANTS_NEW)\n");
	parse_STATEMENT();
	parse_STATEMENTS_NEW();
}

void parse_STATEMENTS_NEW()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_SEMICOLON:
		{
			fprintf(yyoutSyn, "Rule (STATEMANTS_NEW ->; STATEMENT STATEMANTS_NEW)\n");
			parse_STATEMENT();
			parse_STATEMENTS_NEW();
			break;
		}

		case TOKEN_KEYWORD_END:
		{
			fprintf(yyoutSyn, "Rule (STATEMANTS_NEW -> epsilon(END))\n");
			back_token();
			break;
		}
		
		case TOKEN_RIGHT_CURLY_BRACKETS:
		{
			fprintf(yyoutSyn, "Rule (STATEMANTS_NEW -> epsilon)\n");
			back_token();
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_SEMICOLON,TOKEN_KEYWORD_END,TOKEN_RIGHT_CURLY_BRACKETS] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(STATEMANTS_NEW);
		}
	}

}

void parse_STATEMENT()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_BLOCK:
		{
			fprintf(yyoutSyn, "Rule (STATEMENT -> BLOCK)\n");
			back_token();
			parse_BLOCK();
			break;
		}
		case TOKEN_BREAK:
		{
			fprintf(yyoutSyn, "Rule (STATEMENT -> break)\n");
			break;
		}
		case TOKEN_SWITCH:
		{
			fprintf(yyoutSyn, "Rule (STATEMENT -> switch)\n");
			match(TOKEN_LEFT_PARENTHESES);
			parse_KEY();
			match(TOKEN_RIGHT_PARENTHESES);
			match(TOKEN_LEFT_CURLY_BRACKETS);
			parse_CASE_LIST();
			match(TOKEN_SEMICOLON);
			match(TOKEN_DEFAULT);
			match(TOKEN_COLON);
			parse_STATEMENTS();
			match(TOKEN_RIGHT_CURLY_BRACKETS);
			break;
		}
		case TOKEN_ID:
		{
			fprintf(yyoutSyn, "Rule (STATEMENT -> VAR_ELEMENT = EXPRESSION)\n");
			back_token();
			parse_VAR_ELEMENT();
			match(TOKEN_ASSIGNMENT);
			parse_EXPRESSION();
			break;			
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_BLOCK,TOKEN_SWITCH,TOKEN_BREAK,TOKEN_ID] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(STATEMENT);
		}
	}

}

void parse_VAR_ELEMENT()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{

		case TOKEN_ID:
		{
			Token* secondToken = peek();
			if (secondToken->kind == TOKEN_LEFT_BRACKETS )
			{
			fprintf(yyoutSyn, "(VAR_ELEMENT -> VAR_ELEMENT_NEW)\n");
			parse_VAR_ELEMENT_NEW();
			}

			else if(secondToken->kind == TOKEN_KEYWORD_END/*ADDED BY SAHAR FOR LINE 71 PROBLAM*/ || secondToken->kind == TOKEN_RIGHT_BRACKETS || secondToken->kind == TOKEN_SEMICOLON || secondToken->kind == TOKEN_ASSIGNMENT || secondToken->kind == TOKEN_AR_OP_ADD || secondToken->kind == TOKEN_AR_OP_SUB || secondToken->kind == TOKEN_AR_OP_MULTI || secondToken->kind == TOKEN_AR_OP_DIVIDE)
			{
			fprintf(yyoutSyn, "(VAR_ELEMENT -> epsilon)\n");
			parse_VAR_ELEMENT_NEW();
			}
			
			else
			{
			fprintf(yyoutSyn, "(VAR_ELEMENT -> FIELD_ACCESS)\n");
			back_token();
			parse_FIELD_ACCESS();
			}
		}
		break;

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_ID] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(VAR_ELEMENT);
		}
		
	}
}

void parse_VAR_ELEMENT_NEW()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_LEFT_BRACKETS: 
		{
			fprintf(yyoutSyn, "(VAR_ELEMENT_NEW  -> [EXPRESSION])\n");
			parse_EXPRESSION();
			match(TOKEN_RIGHT_BRACKETS);
			break;
		}

		case TOKEN_ASSIGNMENT:
		{
			fprintf(yyoutSyn, "(VAR_ELEMENT_NEW  -> epsilon(=))\n");
			back_token();
			break;
		}

		case TOKEN_RIGHT_BRACKETS:
		{
			fprintf(yyoutSyn, "(VAR_ELEMENT_NEW  -> epsilon(]))\n");
			back_token();
			break;

		}

		case TOKEN_SEMICOLON:
		{
			fprintf(yyoutSyn, "(VAR_ELEMENT_NEW  -> epsilon(;))\n");
			back_token();
			break;
		}
		case TOKEN_AR_OP_ADD:  
		case TOKEN_AR_OP_SUB:  
		case TOKEN_AR_OP_MULTI: 
		case TOKEN_AR_OP_DIVIDE:
		{
			fprintf(yyoutSyn, "(VAR_ELEMENT_NEW  -> epsilon)\n");
			break;
		}
		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_LEFT_BRACKETS,TOKEN_ASSIGNMENT,TOKEN_AR_OP_ADD,TOKEN_AR_OP_SUB,TOKEN_AR_OP_MULTI,TOKEN_AR_OP_DIVIDE] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(VAR_ELEMENT_NEW);
		}
	
	}
}

void parse_FIELD_ACCESS()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_ID:
		{
			fprintf(yyoutSyn, "(FIELD_ACCESS  -> id.FIELD_ACCESS)\n");
			match(TOKEN_DOT);
			parse_FIELD_ACCESS_NEW();
			break;
		}
		/*21.1.20 --> No need
		case TOKEN_ASSIGNMENT:
		case TOKEN_RIGHT_BRACKETS:
		{
			fprintf(yyoutSyn, "(FIELD_ACCESS  -> epsilon)\n");
			back_token();
			break;
		}*/

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_ID] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(FIELD_ACCESS);

		}
	}

}

void parse_FIELD_ACCESS_NEW()
{
	currentToken = next_token();
	switch (currentToken->kind)
	{
		case TOKEN_ID:
		{
			Token* secondToken = peek();
			if (secondToken->kind == TOKEN_DOT)
			{
			fprintf(yyoutSyn, "(FIELD_ACCESS_NEW  -> FIELD_ACCESS)\n");
			back_token();
			}

			else
			{
			fprintf(yyoutSyn, "(FIELD_ACCESS_NEW  -> id\n");
			}
		}
		break;

		default:
			{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_ID(and after TOKEN_DOT)] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(FIELD_ACCESS);
			}
	}
}

void parse_EXPRESSION()
{

			fprintf(yyoutSyn, "Rule (EXPRESSION -> SIMPLE_EXPRASSION EXPRASSION_NEW)\n");
			parse_SIMPLE_EXPRASSION();
			parse_EXPRASSION_NEW();
}

void parse_EXPRASSION_NEW()
{
	currentToken = next_token();
	switch(currentToken->kind)
	{
		case TOKEN_AR_OP_ADD:
		{
			fprintf(yyoutSyn, "Rule (EXPRASSION_NEW -> AR_OP_ADD EXPRESSION)\n");
			parse_EXPRESSION();
			break;
		}
		case TOKEN_AR_OP_SUB:
		{
			fprintf(yyoutSyn, "Rule (EXPRASSION_NEW -> AR_OP_SUB EXPRESSION)\n");
			parse_EXPRESSION();
			break;
		}
		case TOKEN_AR_OP_MULTI:
		{
			fprintf(yyoutSyn, "Rule (EXPRASSION_NEW -> AR_OP_MULTI EXPRESSION)\n");
			parse_EXPRESSION();
			break;
		}
		case TOKEN_AR_OP_DIVIDE:
		{
			fprintf(yyoutSyn, "Rule (EXPRASSION_NEW -> AR_OP_DIVIDE EXPRESSION)\n");
			parse_EXPRESSION();
			break;
		}

		case TOKEN_ID:
		{
			fprintf(yyoutSyn, "Rule (EXPRASSION_NEW -> epsilon(ID))\n");
			back_token();
			parse_EXPRESSION();
			break;
		}

		case TOKEN_SEMICOLON:
		case TOKEN_KEYWORD_END:
		case TOKEN_RIGHT_BRACKETS:
		case TOKEN_RIGHT_CURLY_BRACKETS:
		{
			fprintf(yyoutSyn, "Rule (EXPRASSION_NEW -> epsilon)\n");
			back_token();
			break;				
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_AR_OP_ADD, TOKEN_AR_OP_SUBL, TOKEN_AR_OP_MULTI, TOKEN_AR_OP_DIVIDE,TOKEN_SEMICOLON,OKEN_KEYWORD_END,TOKEN_RIGHT_BRACKETS,TOKEN_RIGHT_CURLY_BRACKETS] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(EXPRASSION_NEW);
		}
	}
}

void parse_SIMPLE_EXPRASSION(){
	currentToken = next_token();
	switch(currentToken->kind)
	{
		case TOKEN_REAL_NUM:
		{
			fprintf(yyoutSyn, "Rule (SIMPLE_EXPRASSION-> real_num)\n");
			break;
		}

		case TOKEN_INTEGER_NUM:
		{
			fprintf(yyoutSyn, "Rule (SIMPLE_EXPRASSION-> int_num )\n");
			break;
		}

		case TOKEN_ID:
		{
			fprintf(yyoutSyn, "Rule (SIMPLE_EXPRASSION-> VAR_ELEMENT)\n");
			back_token();
			parse_VAR_ELEMENT();
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_INTEGER_NUM, TOKEN_REAL_NUM, TOKEN_ID] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(SIMPLE_EXPRASSION);
		}
	}
}

void parse_KEY()
{
			fprintf(yyoutSyn, "Rule (KEY -> VAR_ELEMENT)\n");
			parse_VAR_ELEMENT();
}

void parse_CASE_LIST()
{
			fprintf(yyoutSyn, "Rule (CASE_LIST -> CASE CASE_LIST_NEW)\n");
			parse_CASE();
			parse_CASE_LIST_NEW();
}

void parse_CASE_LIST_NEW(){
	currentToken = next_token();
	switch(currentToken->kind)
	{
		case TOKEN_CASE:
		{
			fprintf(yyoutSyn, "Rule (CASE_LIST_NEW -> CASE CASE_LIST_NEW)\n");
			back_token();
			parse_CASE();
			parse_CASE_LIST_NEW();
			break;
		}

		case TOKEN_SEMICOLON:
		{
			fprintf(yyoutSyn, "Rule (CASE_LIST_NEW -> epsilon)\n");
			back_token();
			break;
		}
		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_CASE,TOKEN_SEMICOLON] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(CASE_LIST_NEW);
		}
	}
}

void parse_CASE()
{
	currentToken = next_token();
	switch(currentToken->kind)
	{
		case TOKEN_CASE:
		{
			fprintf(yyoutSyn, "Rule (CASE -> case KEY_VALUE : { STATEMENTS })\n");
			parse_KEY_VALUE();
			match(TOKEN_COLON);
			match(TOKEN_LEFT_CURLY_BRACKETS);
			parse_STATEMENTS();
			match(TOKEN_RIGHT_CURLY_BRACKETS);
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_CASE] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(CASE);
		}
	}
}

void parse_KEY_VALUE(){
	currentToken = next_token();
	switch(currentToken->kind)
	{
		case TOKEN_INTEGER_NUM:
		{
			fprintf(yyoutSyn, "Rule (KEY_VALUE -> int_num)\n");
			break;
		}
		case TOKEN_ID:
		{
			fprintf(yyoutSyn, "Rule (KEY_VALUE -> id)\n");
			break;
		}

		default:
		{
			fprintf(yyoutSyn, "Expected: one of tokens [TOKEN_ID, TOKEN_INTEGER_NUM] at line %d, Actual token: %s, lexeme %s\n", currentToken->lineNumber, convertFromTokenKindToString(currentToken->kind), currentToken->lexeme);
			recoveryFromError(KEY_VALUE);
		}	
	}
}

int isFollowOfVariable(Grammer variable, eTOKENS kind)
{
	switch (variable)
	{
		case PROGRAM:
		{
			switch(kind)
			{
				case TOKEN_EOF: return 1;
				default: return 0;
			}
		}	

		case BLOCK:
		{
			switch(kind)
			{
				case TOKEN_EOF:
				case TOKEN_SEMICOLON:
				case TOKEN_KEYWORD_END: return 1;
				default: return 0;
			}
		}

		case DECLARATIONS:
		case DECLARATIONS_NEW:
		{
			switch(kind)
			{
				case TOKEN_BEGIN: return 1;
				default: return 0;
			}
		}

		case DECLARATION:
		case TYPE_DECLARATION:
		case TYPE_INDICATOR:
		case STRUCTURE_TYPE:
		case ENUM_TYPE:
		{
			switch(kind)
			{
				case TOKEN_SEMICOLON:
				case TOKEN_BEGIN: return 1;
				default: return 0;
			}
		}

		case VAR_DECLARATION:
		case VAR_DECLARATION_NEW:
		case SIMPLE_TYPE:
		{
			switch(kind)
			{
				case TOKEN_SEMICOLON:
				case TOKEN_BEGIN: 
				case TOKEN_RIGHT_CURLY_BRACKETS: return 1;
				default: return 0;
			}
		}

		case SIZE:
		{
			switch(kind)
			{
				case TOKEN_RIGHT_BRACKETS: return 1;
				default: return 0;
			}
		}
	


		case ID_LIST:
		case ID_LIST_NEW:
		case FIELDS:
		case FIELDS_NEW:
		{
			switch(kind)
			{
				case TOKEN_RIGHT_CURLY_BRACKETS: return 1;
				default: return 0;
			}
		}

		case CASE_LIST:
		case CASE_LIST_NEW:
		{
			switch(kind)
			{
				case TOKEN_SEMICOLON: return 1;
				default: return 0;
			}
		}

		case FIELD:
		{
			switch(kind)
			{
				case TOKEN_RIGHT_CURLY_BRACKETS:
				case TOKEN_SEMICOLON: return 1;
				default: return 0;
			}
		}

		case STATEMENTS:
		case STATEMANTS_NEW:
		{
			switch(kind)
			{
				case TOKEN_KEYWORD_END:
				case TOKEN_RIGHT_CURLY_BRACKETS: return 1;
				default: return 0;
			}
		}

		case STATEMENT:
		{
			switch(kind)
			{
				case TOKEN_SEMICOLON:
				case TOKEN_KEYWORD_END:
				case TOKEN_RIGHT_CURLY_BRACKETS: return 1;
				default: return 0;
			}
		}

		case VAR_ELEMENT:
		case VAR_ELEMENT_NEW:
		case FIELD_ACCESS:
		case FIELD_ACCESS_NEW:
		{
			switch(kind)
			{
				case TOKEN_ASSIGNMENT:
				case TOKEN_RIGHT_BRACKETS:
				case TOKEN_RIGHT_PARENTHESES:
				case TOKEN_RIGHT_CURLY_BRACKETS:
				case TOKEN_SEMICOLON:
				case TOKEN_KEYWORD_END:
				case TOKEN_AR_OP_ADD:
				case TOKEN_AR_OP_SUB:
				case TOKEN_AR_OP_MULTI:		
				case TOKEN_AR_OP_DIVIDE: return 1;
				default: return 0;
			}
		}
		
		case KEY:
		{
			switch(kind)
			{
				case TOKEN_RIGHT_PARENTHESES: return 1;
				default: return 0;
			}
		}

		case EXPRESSION:
		case EXPRASSION_NEW:
		{
			switch(kind)
			{
				case TOKEN_SEMICOLON:
				case TOKEN_KEYWORD_END:
				case TOKEN_RIGHT_BRACKETS:
				case TOKEN_RIGHT_CURLY_BRACKETS: return 1;
				default: return 0;
			}
		}

		case SIMPLE_EXPRASSION:
		{
			switch(kind)
			{
				case TOKEN_SEMICOLON:
				case TOKEN_KEYWORD_END:
				case TOKEN_RIGHT_BRACKETS:
				case TOKEN_RIGHT_CURLY_BRACKETS:
				case TOKEN_AR_OP_ADD:
				case TOKEN_AR_OP_SUB:
				case TOKEN_AR_OP_MULTI:
				case TOKEN_AR_OP_DIVIDE: return 1;
				default: return 0;
			}
		}

		case CASE:
		{
			switch(kind)
			{
				case TOKEN_CASE:
				case TOKEN_SEMICOLON: return 1;
				default: return 0;
			}
		}

		case KEY_VALUE:
		{
			switch(kind)
			{
				case TOKEN_COLON: return 1;
				default: return 0;
			}
		}
		



		


	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	}
}

void recoveryFromError(Grammer i_Variable)
{
	while (!isFollowOfVariable(i_Variable, currentToken->kind) && currentToken->kind != TOKEN_EOF)
	{
		currentToken = next_token();
	}

	back_token();
}
