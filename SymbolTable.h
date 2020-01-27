#pragma once
#include <string.h>
#include <math.h>


// Types
#define REAL 1
#define INTEGER 2
#define ARRAY 3
#define VOID 4

// Roles
#define VAR 500
#define FUNCTION 501

#define HASH_ARRAY_SIZE 201
#define FUNCTIONS_ARRAY 200
#define FUNCTIONS_VARIABLES_ARRAY 50
#define ERROR_TYPES 6

typedef enum eErrorTypes
{
	REAL_TO_INTEGER = 1,
	RIGHT_VARIABLE_UNDEFINED,
	ARRAY_INDEX_OUT_OF_RANGE,
	USING_ARRAY_WITHOUT_SQUARE_BRACKET,
	DIVIDED_BY_ZERO

}eErrorTypes;

typedef struct ErrorExpression
{
	int lineNumber;
	char* variableName;
	enum eErrorTypes error;
}ErrorExpression;

typedef struct SymTableEntry {
	char *name;
	int size;
	int countInstance;
	int roleType;
	int type;
	int subType;
	int defineInLineNumber;
	ErrorExpression errorsExpressions[ERROR_TYPES];
	struct SymTableEntry *next;
}SymTableEntry;

typedef struct FunctionCell {
	char *name;
	char* variables[FUNCTIONS_VARIABLES_ARRAY];
	int returnType;
	int lineNumber;
	int totalVariables;
}FunctionCell;


typedef struct SymTable {
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
SymTableEntry* find(char *name, SymTable* current_ptr);
SymTableEntry* lookup(char *name, SymTable* current_ptr);
long HashFoldingFunction(char *name);

void set_type(SymTableEntry* entry, int type);
int get_type(SymTableEntry*);

void set_roleType(SymTableEntry*, int roleType);
int get_roleType(SymTableEntry*);

void set_subType(SymTableEntry*, int);
int get_subType(SymTableEntry*);

void set_size(SymTableEntry*, int size);
int get_size(SymTableEntry*);

void setLineNumber(SymTableEntry* entry, int lineNumber);

void setError(int error, int line, char* VariableName);
void printErrors(int);

int ifFunctionExist(char* name);
void resetFunctionsData();
int getIndexFunction(char* name);
int findFunction(char* name);
int isIdExistInFunction(char *name);