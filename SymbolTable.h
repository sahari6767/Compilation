#pragma once
#include <string.h>
#include <math.h>

// Roles
#define ROLE_VAR 100
#define ROLE_ENUM 101
#define ROLE_STRUCT 10

// Types
#define TYPE_REAL 1
#define TYPE_INTEGER 2
#define TYPE_ARRAY 3

#define HASH_ARRAY_SIZE 255

typedef enum elm_type { TYPE_NULL, integer, real } elm_type;

typedef struct arrayField {
    char *fieldName;
    struct arrayField *nextField;
} arrayField;

typedef struct structFields {
    struct arrayField *nextField;
} structValues;

typedef struct enumValue {
    char *enumValue;
    struct enumValue *nextValue;
} enumValue;

typedef struct enumValues {
    struct enumValue *nextValue;
} enumValues;

typedef struct arrayType {
    int size;
    int elementsType;
} arrayType;

typedef enum eErrorTypes {
	REAL_TO_INTEGER = 1,
	RIGHT_VARIABLE_UNDEFINED,
	ARRAY_INDEX_OUT_OF_RANGE,
	USING_ARRAY_WITHOUT_SQUARE_BRACKET,
	DIVIDED_BY_ZERO
} eErrorTypes;

typedef struct ErrorExpression {
	int lineNumber;
	char* variableName;
	enum eErrorTypes error;
} ErrorExpression;

typedef struct SymTableEntry {
	char *name;
	int roleType;
	int type;
	int size;
    int numInstances;
	int defineInLineNumber;
    int instances;
	ErrorExpression errorsExpressions[ERROR_TYPES];
	struct SymTableEntry *next;
} SymTableEntry;

typedef struct SymTable {
	SymTableEntry *HashingTable[HASH_ARRAY_SIZE];
	struct SymTable *father;
} SymTable;

SymTable *cur_table;
SymTableEntry *cur_entry;

SymTable* make_table(SymTable* current_ptr);
SymTable* pop_table(SymTable* current_tab);
SymTableEntry insert(SymTable* current_ptr, char *name);
SymTableEntry lookup(SymTable* current_ptr, char *name);
SymTableEntry find(SymTable* current_ptr, char *name);

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