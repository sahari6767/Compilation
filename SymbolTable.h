#pragma once
#include <string.h>
#include <math.h>

// Types
#define TYPE_REAL 1
#define TYPE_INTEGER 2
#define TYPE_ARRAY 3

// Roles
#define ROLE_VAR 100
#define ROLE_ENUM 101
#define ROLE_STRUCT 102

typedef enum eErrorTypes{
	REAL_TO_INTEGER = 1,
	RIGHT_VARIABLE_UNDEFINED,
	ARRAY_INDEX_OUT_OF_RANGE,
	USING_ARRAY_WITHOUT_SQUARE_BRACKET,
	DIVIDED_BY_ZERO
}eErrorTypes;

typedef struct ErrorExpression{
	int lineNumber;
	char* variableName;
	enum eErrorTypes error;
}ErrorExpression;

typedef struct SymTableEntry{
	char *name;
	int size;
    int numInstances;
	int roleType;
	int type;
	int defineInLineNumber; // todo Maybe move to error
	ErrorExpression errorsExpressions[ERROR_TYPES];
	struct SymTableEntry *next;
}SymTableEntry;

typedef struct SymTable{
	SymTableEntry *HashingTable[HASH_ARRAY_SIZE];
	struct SymTable *father;
}SymTable;

SymTable *cur_table;
SymTableEntry *cur_entry;
FunctionCell funcArray[FUNCTIONS_ARRAY];
int index;
int variableIndex;

SymTable* make_table(SymTable* current_ptr);
SymTable* pop_table(SymTable* current_tab);
SymTableEntry* insert(char *name, SymTable* current_ptr);
SymTableEntry* lookup(char *name, SymTable* current_ptr);
SymTableEntry* find(char *name, SymTable* current_ptr);

void set_type(SymTableEntry* entry, int type);
int get_type(SymTableEntry*);

long HashFoldingFunction(char *name);

void set_roleType(SymTableEntry*, int roleType);
int get_roleType(SymTableEntry*);

void set_size(SymTableEntry*, int size);
int get_size(SymTableEntry*);

void setLineNumber(SymTableEntry* entry, int lineNumber);

void setError(int error, int line, char* VariableName);
void printErrors(int);