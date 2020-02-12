#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "SymbolTable.h"
#define FOLDING 2

extern FILE *yyoutSem;

// Use folding on a string, summed 2 bytes at a time (copy with changes from: https://research.cs.vt.edu/AVresearch/hashing/strings.php)
long HashFoldingFunction(char *var_name) {
	int intLength = strlen(var_name) / FOLDING;
	long sum = 0;
	char c[FOLDING];

	for (int j = 0; j < intLength; j++)
	{
		long mult = 1;
		strncpy(c, var_name + j * FOLDING, FOLDING);
		for (int k = 0; k < FOLDING; k++)
		{
			sum += c[k] * mult;
			mult *= 256;
		}
	}

	if ((strlen(var_name) % 2) != 0)
		sum += var_name[strlen(var_name) - 1];

	return((abs(sum)) % HASH_ARRAY_SIZE);
}

SymTable* make_table(SymTable* current_table) {
	SymTable *SymbolTable = (SymTable*)malloc(sizeof(SymTable));
	SymbolTable->father = current_table; // connect between the tables
	for (int i = 0; i < HASH_ARRAY_SIZE; i++)
	{
		SymbolTable->HashingTable[i] = NULL; // init hash array
	}

	return SymbolTable;
}

SymTable* pop_table(SymTable* current_table) {
	return currentTable->father;
}

// TODO: still need to fix the insert method. Waitting for Mark's email back
SymTableEntry insert(SymTable* current_table, char* id_name) {
	long index = HashFoldingFunction(var_name);
	SymTableEntry **entry = &(currentTable->HashingTable[index]);
	if (!(*entry))
	{
		*entry = (SymTableEntry*) malloc(sizeof(SymTableEntry));
		(*entry)->name = (char*) malloc(sizeof(char) * strlen(var_name));
		strcpy((*entry)->name, var_name);
        (*entry) -> type = var_type
		(*entry)->next = NULL;
		(*entry)->errorsExpressions[RIGHT_VARIABLE_UNDEFINED].variableName = "";
		(*entry)->errorsExpressions[REAL_TO_INTEGER].variableName = "";
		return *entry;
	}

	while ((*entry)->next)
	{
		// that for the case that the index contain more then one ID then need to check on the entire list
		if (!strcmp(var_name, (*entry)->name))
		{	
			return NULL;
		}
		
		*entry = (*entry)->next;
	}

	if (!strcmp(var_name, (*entry)->name))
	{
		return NULL;
	}

	(*entry) = (SymTableEntry*)malloc(sizeof(SymTableEntry));
	(*entry)->name = (char*)malloc(sizeof(char)*strlen(var_name));
	strcpy((*entry)->name, var_name);
	(*entry)->next = NULL;
	(*entry)->errorsExpressions[RIGHT_VARIABLE_UNDEFINED].variableName = "";
	(*entry)->errorsExpressions[REAL_TO_INTEGER].variableName = "";
	(*entry)->numInstances = 0;
	return (*entry);
}

SymTableEntry lookup(SymTable* current_table, char* id_name){
	long index = HashFoldingFunction(idName);
	SymTableEntry *entry = currentTable->HashingTable[index];
	while (entry)
	{
		if (!strcmp(idName, entry->name)) {
			return *entry;
		}
		entry = entry->next;
	}
	return NULL;
}

SymTableEntry find(SymTable* current_table, char* id_name){
	SymTableEntry *entry;
	while (current_table)
	{
		if ((entry = lookup(current_table, id_name))) {
			return *entry;
        } else {
            // not present in this table. Check father's table.
			current_table = pop_table(current_table);
        }
	}

	return NULL;
}

void set_id_type(SymTableEntry currentEntry, elm_type id_type){
	if (currentEntry == NULL) {
        return;
    }

    (&currentEntry) -> type = id_type;
}

elm_type get_id_type(SymTableEntry currentEntry){
	return (&currentEntry)->type;
}

void setError(int error, int lineNumber, char* variableName){
	cur_entry->errorsExpressions[error].error = error;
	cur_entry->errorsExpressions[error].lineNumber = lineNumber;
	cur_entry->errorsExpressions[error].variableName = variableName;
}

void setLineNumber(SymTableEntry* current_entry, int lineNumber){
	if (currentEntry != NULL) {
		currentEntry->defineInLineNumber = lineNumber;
    }
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
