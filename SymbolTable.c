#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "SymbolTable.h"
#define FOLDING 2

extern FILE *yyoutSem;

// Use folding on a string, summed 2 bytes at a time (copy with changes from: https://research.cs.vt.edu/AVresearch/hashing/strings.php)
long HashFoldingFunction(char *TokenIdName){
	int intLength = strlen(TokenIdName) / FOLDING;
	long sum = 0;
	char c[FOLDING];

	for (int j = 0; j < intLength; j++)
	{
		long mult = 1;
		strncpy(c, TokenIdName + j * FOLDING, FOLDING);
		for (int k = 0; k < FOLDING; k++)
		{
			sum += c[k] * mult;
			mult *= 256;
		}
	}

	if ((strlen(TokenIdName) % 2) != 0)
		sum += TokenIdName[strlen(TokenIdName) - 1];

	return((abs(sum)) % HASH_ARRAY_SIZE);
}

SymTable* make_table(SymTable* current_ptr){
	SymTable *SymbolTable = (SymTable*)malloc(sizeof(SymTable));
	SymbolTable->father = current_ptr; // connect between the tables
	for (int i = 0; i < HASH_ARRAY_SIZE; i++)
	{
		SymbolTable->HashingTable[i] = NULL; // init hash array
	}

	return SymbolTable;
}

SymTableEntry* lookup(char *idName, SymTable* currentTable){
	long index = HashFoldingFunction(idName);
	SymTableEntry *entry = currentTable->HashingTable[index];
	while (entry)
	{
		if (!strcmp(idName, entry->name))
		{
			return entry;
		}
		entry = entry->next;
	}

	return NULL;
}

SymTableEntry* find(char *IdNameToFind, SymTable* current_table){
	SymTableEntry *entry;
	while (current_table)
	{
		if ((entry = lookup(IdNameToFind, current_table)))
			return entry; // was found
		else
			current_table = pop_table(current_table); // not found in current table, find in the father table
	}

	return NULL;
}

// insert variable / func to symbol table (return NULL in case that the already in the table)
SymTableEntry* insert(char *TokenIdName, SymTable* currentTable){

	long index = HashFoldingFunction(TokenIdName);
	SymTableEntry **entry = &(currentTable->HashingTable[index]);
	if (!(*entry))
	{
		*entry = (SymTableEntry*)malloc(sizeof(SymTableEntry));
		(*entry)->name = (char*)malloc(sizeof(char) * strlen(TokenIdName));
		strcpy((*entry)->name, TokenIdName);
		(*entry)->next = NULL;
		(*entry)->countInstance = 0;
		(*entry)->errorsExpressions[RIGHT_VARIABLE_UNDEFINED].variableName = "";
		(*entry)->errorsExpressions[REAL_TO_INTEGER].variableName = "";
		return *entry;
	}

	while ((*entry)->next)
	{
		// that for the case that the index contain more then one ID then need to check on the entire list
		if (!strcmp(TokenIdName, (*entry)->name))
		{	
			return NULL;
		}
		
		*entry = (*entry)->next;
	}

	if (!strcmp(TokenIdName, (*entry)->name))
	{
		return NULL;
	}

	(*entry) = (SymTableEntry*)malloc(sizeof(SymTableEntry));
	(*entry)->name = (char*)malloc(sizeof(char)*strlen(TokenIdName));
	strcpy((*entry)->name, TokenIdName);
	(*entry)->next = NULL;
	(*entry)->errorsExpressions[RIGHT_VARIABLE_UNDEFINED].variableName = "";
	(*entry)->errorsExpressions[REAL_TO_INTEGER].variableName = "";
	(*entry)->countInstance = 0;
	return (*entry);
}

SymTable* pop_table(SymTable* currentTable){
	return currentTable->father;
}

void set_size(SymTableEntry* currentEntry, int size){
	if (currentEntry != NULL)
		currentEntry->size = size;
}

int get_size(SymTableEntry* currentEntry){
	return currentEntry->size;
}

void set_type(SymTableEntry* currentEntry, int type){
	if (currentEntry != NULL)
		currentEntry->type = type;
}

int get_type(SymTableEntry* currentEntry){
	return currentEntry->type;
}

void set_roleType(SymTableEntry* currentEntry, int roleType){
	if (currentEntry != NULL)
		currentEntry->roleType = roleType;
}

int get_roleType(SymTableEntry* currentEntry){
	return currentEntry->roleType;
}

void set_subType(SymTableEntry* currentEntry, int idsub){
	if (currentEntry != NULL)
		currentEntry->subType = idsub;
}

int get_subType(SymTableEntry* currentEntry){
	return currentEntry->subType;
}

void setLineNumber(SymTableEntry* currentEntry, int lineNumber){
	if (currentEntry != NULL)
		currentEntry->defineInLineNumber = lineNumber;
}

void setError(int error, int lineNumber, char* variableName){
	cur_entry->errorsExpressions[error].error = error;
	cur_entry->errorsExpressions[error].lineNumber = lineNumber;
	cur_entry->errorsExpressions[error].variableName = variableName;
}

void printErrors(int lineNumber){
	int i;
	if (cur_entry)
	{
		for (i = 0; i < ERROR_TYPES; i++)
		{
			switch (cur_entry->errorsExpressions[i].error)
			{
			case REAL_TO_INTEGER:
				fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: Can't convert [%s] real to integer\n", "C007",
					cur_entry->errorsExpressions[i].lineNumber, cur_entry->errorsExpressions[i].variableName);
				break;
			case RIGHT_VARIABLE_UNDEFINED:
				fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: The variable(s) [%s] at the expression are undefiend\n", "C008",
					cur_entry->errorsExpressions[i].lineNumber, cur_entry->errorsExpressions[i].variableName);
				break;
			case ARRAY_INDEX_OUT_OF_RANGE:
				fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: The Array '%s' in expression is out of range\n", "C009",
					cur_entry->errorsExpressions[i].lineNumber, cur_entry->errorsExpressions[i].variableName);
				break;
			case USING_ARRAY_WITHOUT_SQUARE_BRACKET:
				fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: The Array '%s' can't use on expression to entire array\n", "C010",
					cur_entry->errorsExpressions[i].lineNumber, cur_entry->errorsExpressions[i].variableName);
				break;
			case DIVIDED_BY_ZERO:
				fprintf(yyoutSem, "Error   #%s \t Line number:%3d\t Description: Can't be divided by zero\n", "C011",
					cur_entry->errorsExpressions[i].lineNumber);
				break;
			default:
				break;
			}
			cur_entry->errorsExpressions[i].error = 0;
			cur_entry->errorsExpressions[i].variableName = "";
		}
	}
}

void resetFunctionsData(){
	index = 0;
	variableIndex = 0;
	for (int i = 0; i < FUNCTIONS_ARRAY; i++)
	{
		funcArray[i].lineNumber = 0;
		funcArray[i].name = "";
		funcArray[i].returnType = -1;
		funcArray[i].totalVariables = 0;
		for (int j = 0; j < FUNCTIONS_VARIABLES_ARRAY; j++)
		{
			funcArray[i].variables[j] = "";
		}
	}
}

int ifFunctionExist(char* name){
	for (int i = 0; i < FUNCTIONS_ARRAY; i++)
	{
		if (funcArray[i].name)
		{
			if (!strcmp(funcArray[i].name, name))
			{
				return 1; // found
			}
		}

	}

	return 0; // not found
}

void howMuchVariables(){
	int count = 0;
	SymTableEntry *entry;
	if (cur_table)
	{
		for (int i = 0; i < HASH_ARRAY_SIZE; i++)
		{
			entry = cur_table->HashingTable[i];
			while (entry)
			{
				if (entry)
				{
					count++;
					funcArray[index].variables[variableIndex] = entry->name;
					variableIndex++;
					entry = entry->next;
				}
				
			}
		}
	}
	variableIndex = 0;
	funcArray[index].totalVariables = count;
}

int isIdExistInFunction(char *name){
	for (int i = 0; i < FUNCTIONS_VARIABLES_ARRAY; i++)
	{
		if (strcmp(funcArray[index].variables[i], name) == 0)
		{
			return 1;
		}
	}

	return 0;
}

int findFunction(char* name){
	for (int i = 0; i < FUNCTIONS_ARRAY; i++)
	{
		if (strcmp(funcArray[i].name, name) == 0)
		{
			return 1;
		}
	}

	return 0;
}

int getIndexFunction(char* name){
	for (int i = 0; i < FUNCTIONS_ARRAY; i++)
	{
		if (strcmp(funcArray[i].name, name) == 0)
		{
			return i;
		}
	}

	return -1;
}


