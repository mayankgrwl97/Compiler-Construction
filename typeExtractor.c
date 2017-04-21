#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symboltable.h"
#include "hashtable.h"
#include "token.h"
#include "parserDef.h"
#include "mainsymboltable.h"
#include "idsymboltable.h"
#include "typeExtractor.h"

char* findTypeString(int n)
{
	if(n == integer)	
		return "integer";
	if(n == real)
		return "real";
	if(n == boolean)
		return "boolean";
	return "ERROR";
}

int gettype(stacknode* type)	// expects ID->type
{
	if(strcmp(type->ntortinfo->str, "INTEGER") == 0)
		return integer;
	else if(strcmp(type->ntortinfo->str, "REAL") == 0)
		return real;
	else if(strcmp(type->ntortinfo->str, "BOOLEAN") == 0)
		return boolean;
	else
		return gettype(type->child->sibling);		// @@@@ array operations should be checked separately.@@@@
}

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
			if(curr->child->idst == NULL)
				return error;
			else{
				idsymboltablenode* temp = getidsymboltablenode(curr->child->tokinfo->lexeme, curr->child->idst);
				return gettype(temp->type);
			}
		}
	}

	if((strcmp(curr->ntortinfo->str, "TRUE") == 0) || (strcmp(curr->ntortinfo->str, "FALSE") == 0))
		return boolean;

	if(strcmp(curr->ntortinfo->str, "MINUS") == 0)
	{
		if(curr->sibling != NULL)
			return typeofexpr(curr->sibling);
	}

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
			char* type1 = findTypeString(t1);
			char* type2 = findTypeString(t2);
			printf("ERROR_T at line %d : Type mismatch of %s and %s operands with %s operator.\n", curr->tokinfo->linenumber, type1, type2, curr->ntortinfo->str);
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
			char* type1 = findTypeString(t1);
			char* type2 = findTypeString(t2);
			printf("ERROR_T at line %d : Type mismatch of %s and %s with %s operator.\n", curr->tokinfo->linenumber, type1, type2, curr->ntortinfo->str);
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
			char* type1 = findTypeString(t1);
			char* type2 = findTypeString(t2);
			printf("ERROR_T at line %d : Type mismatch of %s and %s with logical %s.\n", curr->tokinfo->linenumber, type1, type2, curr->ntortinfo->str);
			return error;
		}
	}

	return error;
}

void traverseAST_fortypechecking(stacknode* curr)
{
	if(curr == NULL)
		return;

	// @@@@does the order of traversal matter ?@@@@

	if(strcmp(curr->ntortinfo->str, "<assignmentStmt>") == 0)
	{
		if(curr->child->idst == NULL)
			return;

		idsymboltablenode* temp = getidsymboltablenode(curr->child->tokinfo->lexeme, curr->child->idst);
		int idType = gettype(temp->type);
		if(strcmp(curr->child->sibling->ntortinfo->str, "<lvalueIDStmt>") == 0)
		{
			int exprType = typeofexpr(curr->child->sibling->child);
			if(idType != exprType)
				printf("ERROR_T at line %d : Type mismatch between %s.\n", curr->child->tokinfo->linenumber, curr->child->tokinfo->lexeme);
		}
		else
		{
			int exprType = typeofexpr(curr->child->sibling);
			if(idType != exprType)
				printf("ERROR_T at line %d : Type mismatch between %s.\n", curr->child->tokinfo->linenumber, curr->child->tokinfo->lexeme);
		}
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<iterativeStmt>") == 0)
	{
		int idType = boolean;
		if(strcmp(curr->child->ntortinfo->str, "WHILE") == 0)
		{
			int exprType = typeofexpr(curr->child->sibling);
			if(idType != exprType)
				printf("ERROR_T at line %d : Type mismatch between %s.\n", curr->child->sibling->tokinfo->linenumber, curr->child->tokinfo->lexeme);
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
