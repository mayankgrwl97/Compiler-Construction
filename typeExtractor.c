#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symboltable.h"
#include "hashtable.h"
#include "token.h"
#include "parserDef.h"
#include "mainsymboltable.h"
#include "idsymboltable.h"

// <level4> --> BO <expression> BC | MINUS BO <expression> BC | <var> | TRUE | FALSE $
// <var> --> ID <whichId> | NUM | RNUM

int gettype(stacknode* type)
{
	if(strcmp(type->ntortinfo->str, "INTEGER") == 0)
		return integer;
	else if(strcmp(type->ntortinfo->str, "REAL") == 0)
		return real;
	else if(strcmp(type->ntortinfo->str, "BOOLEAN") == 0)
		return boolean;
	else
		return gettype(type->child->sibling);
}

// <op1> --> PLUS | MINUS
// <op2> --> MUL | DIV
// <logicalOp> --> AND | OR
// <relationalOp> --> LT | LE | GT | GE | EQ | NE

int typeofexpr(stacknode* curr)
{
	if(strcmp(curr->ntortinfo->str, "<expression>") == 0)
		return typeofexpr(curr->child);

	if(strcmp(curr->ntortinfo->str, "<var>") == 0)
	{
		if(strcmp(curr->child->ntortinfo->str, "NUM") == 0)
			return integer;
		else if(strcmp(curr->child->ntortinfo->str, "RNUM") == 0)
			return real;
		else
		{
			idsymboltablenode* temp = getidsymboltablenode(curr->child->tokinfo->lexeme, curr->child->idst);
			return gettype(temp->type);
		}
	}

	if((strcmp(curr->ntortinfo->str, "TRUE") == 0) || (strcmp(curr->ntortinfo->str, "FALSE") == 0))
		return boolean;

	if(strcmp(curr->ntortinfo->str, "MINUS") == 0)
		return typeofexpr(curr->sibling);

	if((strcmp(curr->ntortinfo->str, "PLUS") == 0) || (strcmp(curr->ntortinfo->str, "MINUS") == 0) || (strcmp(curr->ntortinfo->str, "MUL") == 0) || (strcmp(curr->ntortinfo->str, "DIV") == 0))
	{
		int t1 = typeofexpr(curr->child);
		int t2 = typeofexpr(curr->child->sibling);
		if(t1==t2 && t1==integer)
			return integer;
		else if(t1==t2 && t1==real)
			return real;
		else
		{
			if(t1 == boolean || t2 == boolean)
				printf("ERROR boolean incompatible with arithmetic operator %s \n", curr->ntortinfo->str);
			else if(t1 != t2)
				printf("ERROR type mismatch with arithmetic operator\n");
			return error;
		}
	}

	if((strcmp(curr->ntortinfo->str, "LT") == 0) || (strcmp(curr->ntortinfo->str, "LE") == 0) || (strcmp(curr->ntortinfo->str, "GT") == 0) || (strcmp(curr->ntortinfo->str, "GE") == 0) || (strcmp(curr->ntortinfo->str, "EQ") == 0) || (strcmp(curr->ntortinfo->str, "NE") == 0))
	{
		int t1 = typeofexpr(curr->child);
		int t2 = typeofexpr(curr->child->sibling);
		if(t1==t2 && t1==integer)
			return boolean;
		else if(t1==t2 && t1==real)
			return boolean;
		else
		{
			if(t1 == boolean || t2 == boolean)
				printf("ERROR boolean incompatible with relational operator\n");
			else if(t1 != t2)
				printf("ERROR type mismatch with relational operator\n");
			return error;
		}
	}

	if((strcmp(curr->ntortinfo->str, "AND") == 0) || (strcmp(curr->ntortinfo->str, "OR") == 0))
	{
		int t1 = typeofexpr(curr->child);
		int t2 = typeofexpr(curr->child->sibling);
		if(t1==t2 && t1==boolean)
			return boolean;
		else
		{
			if(t1 != boolean)
				printf("ERROR type incompatible with logical operator\n");
			else if(t2 != boolean)
				printf("ERROR type incompatible with logical operator\n");
			return error;
		}
	}

	return error;
}

void traverseAST_fortypechecking(stacknode* curr)
{
	if(curr == NULL)
		return;

	if(strcmp(curr->ntortinfo->str, "<assignmentStmt>") == 0)
	{
		if(curr->child->idst == NULL){
			printf("%s not correct.\n", curr->child->tokinfo->lexeme);
			return;
		}
		idsymboltablenode* temp = getidsymboltablenode(curr->child->tokinfo->lexeme, curr->child->idst);
		int idType = gettype(temp->type);
		if(strcmp(curr->child->sibling->ntortinfo->str, "<lvalueIDStmt>") == 0)
		{
			int exprType = typeofexpr(curr->child->sibling->child);
			if(idType != exprType)
			{
				printf("ERROR: type mismatch between %s\n", curr->child->tokinfo->lexeme);
			}
		}
		else
		{
			int exprType = typeofexpr(curr->child->sibling);
			if(idType != exprType)
			{
				printf("ERROR: type mismatch between %s\n", curr->child->tokinfo->lexeme);
			}
		}
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<iterativeStmt>") == 0)
	{
		// int temp = typeofexpr(curr);
		// if(temp == 0)
		// 	printf("type of expression is integer\n");
		// else if(temp == 1)
		// 	printf("type of expression is real\n");
		// else if(temp == 2)
		// 	printf("type of expression is boolean\n");
		// else
		// 	printf("ERROR_T");
		// return;
		int idType = 2;		// for boolean
		if(strcmp(curr->child->ntortinfo->str, "WHILE") == 0)
		{
			int exprType = typeofexpr(curr->child->sibling);
			if(idType != exprType)
			{
				printf("ERROR: type mismatch between %s\n", curr->child->tokinfo->lexeme);
			}
		}
		return;
	}

	stacknode* temp = curr->child;
	while(temp != NULL)
	{
		traverseAST_fortypechecking(temp);
		temp = temp->sibling;
	}
	return;
}
