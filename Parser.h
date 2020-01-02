#include "Token.h"

typedef enum Grammer
{
        PROGRAM,
        BLOCK,
        DECLARATIONS,
        DECLARATIONS_NEW,
        DECLARATION,
        VAR_DECLARATION,
        VAR_DECLARATION_NEW,
        SIZE,
        SIMPLE_TYPE,
        TYPE_DECLARATION,
        TYPE_INDICATOR,
        ENUM_TYPE,
        ID_LIST,
        ID_LIST_NEW,
        STRUCTURE_TYPE,
        FIELDS,
        FIELDS_NEW,
        FIELD,
        STATEMENTS,
        STATEMANTS_NEW,
        STATEMENT,
        VAR_ELEMENT,
        VAR_ELEMENT_NEW,
        FIELD_ACCESS,
        EXPRESSION,
        EXPRASSION_NEW,
        SIMPLE_EXPRASSIO,
        KEY,
        CASE_LIST,
        CASE_LIST_NEW,
        CASE,
        KEY_VALUE
}Grammer;

void match(eTOKENS i_ExpectedTokenKind);
void recoveryFromError(Grammer i_Variable);
void mainParser();

void parse_PROGRAM();//CHECK
void parse_BLOCK();//CHECK
void parse_DECLARATIONS();//CHECK
void parse_DECLARATIONS_NEW();//CHECK
void parse_DECLARATION();//CHECK
void parse_VAR_DECLARATION();//CHECK
void parse_VAR_DECLARATION_NEW();
void parse_SIZE();
void parse_SIMPLE_TYPE();
void parse_TYPE_DECLARATION();
void parse_TYPE_INDICATOR();
void parse_ENUM_TYPE();
void parse_ID_LIST();
void parse_ID_LIST_NEW();
void parse_STRUCTURE_TYPE();
void parse_FIELDS();
void parse_FIELDS_NEW();
void parse_FIELD();
void parse_STATEMENTS();
void parse_STATEMENT();
void parse_VAR_ELEMENT();
void parse_VAR_ELEMENT_NEW();
void parse_FIELD_ACCESS();
void parse_EXPRESSION();
void parse_EXPRASSION_NEW();
void parse_SIMPLE_EXPRASSION();
void parse_KEY();
void parse_CASE_LIST();
void parse_CASE_LIST_NEW();
void parse_CASE();
void parse_KEY_VALUE();









