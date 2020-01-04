#include "Tokens.h"

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
        SIMPLE_EXPRASSION,
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
void parse_VAR_DECLARATION_NEW();//CHECK
void parse_SIZE();//CHECK
void parse_SIMPLE_TYPE();//CHECK
void parse_TYPE_DECLARATION();//CHECK
void parse_TYPE_INDICATOR();//CHECK
void parse_ENUM_TYPE();//CHECK
void parse_ID_LIST();//CHECK
void parse_ID_LIST_NEW();//CHECK
void parse_STRUCTURE_TYPE();//CHECK
void parse_FIELDS();//CHECK
void parse_FIELDS_NEW();//CHECK
void parse_FIELD();//CHECK
void parse_STATEMENTS();//CHECK
void parse_STATEMENT();//CHECK
void parse_STATEMENT_NEW();//CHECK
void parse_VAR_ELEMENT();//CHECK
void parse_VAR_ELEMENT_NEW();//CHECK
void parse_FIELD_ACCESS();
void parse_EXPRESSION();////check
void parse_EXPRASSION_NEW();////check
void parse_SIMPLE_EXPRASSION();////check
void parse_KEY();////check
void parse_CASE_LIST();////check
void parse_CASE_LIST_NEW();////check
void parse_CASE();////check
void parse_KEY_VALUE();////check









