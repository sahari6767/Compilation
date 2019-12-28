#ifndef TOKENS_H
#define TOKENS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void match(eTOKENS eType);

void Parse();
void Parse_Program();
void Parse_Block();
void Parse_Declarations();
void Parse_Var_Declaration();
void Parse_Var_Declarations_New();
void Parse_Size();
void Parse_Simple_Type();

void Parse_Type_Declaration();
void Parse_Type_Indicator();
void Parse_Enum_Type();
void Parse_Structure_Type();
void Parse_Id_List();
void Parse_Id_List_New();
void Parse_Fields();
void Parse_Fields_new();
void Parse_Field();



void Parse_Type_Declaration();


void Parse_begin();

void Parse_Var_Declarations();
void Parse_Var_Declaratios_New();
void Parse_Var_Declaratio();
void Parse_Func_Declaratios();
void Parse_Func_Declaratios_New();
void Parse_Func_Declaratio();
void Parse_Param_Declaratios();

void Parse_Statements();
void Parse_Statements_New();
void Parse_Statement();
void Parse_Statement_New();
void Parse_Statement_New2();

void Parse_Variables_List();
void Parse_Variables_List_New();
void Parse_Variable();
void Parse_Variable_New();

void Parse_Returned_Type();

void Parse_Function_Call_New();

void Parse_Expression();
void Parse_Expression_New();

void Error_Handle(eTOKENS*, eTOKENS*, int, int, int);
void Following_Token(eTOKENS*, int);
void Error_Output(Token*, eTOKENS*, int );
void Print_Follows(eTOKENS*, int);
int Is_Token_In_Follows(Token*, eTOKENS*, int);