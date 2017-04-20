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

void endcode()
{
	// printf("\tmov eax, 1\n");
	// printf("\tmov ebx, 1\n");
	// printf("\tint 80h\n");
	printf("\tret\n");
	return;
}

void initialize(/*FILE* fp, */mainsymboltable* globalTable)
{
	printf("SECTION .bss\n");
	// fprintf(fp, "")
	mainsymboltablenode* temp = presentmainsymboltable(globalTable, "program");
	idsymboltable* pt = temp->idst;
	for(int i=0; i<idsymboltablesize; i++)
	{
		if(pt->buckets[i] != NULL)
		{
			idsymboltablenode* curr = pt->buckets[i];
			while(curr != NULL)
			{
				printf("\t%s\tresb %d\n", curr->idlex, 2*curr->widthofid);
				curr = curr->next;
			}
		}
	}

	printf("\n");

	printf("SECTION .data\n");
	printf("get_val: db \"%%d\", 0\n");
	printf("print_val: db \"%%d\", 10, 0\n\n");
	printf("SECTION .text\n");
	printf("extern printf\n");
	printf("extern scanf\n");
	printf("\tGLOBAL main\n\n");

	printf("main:\n");
	return;
}

void code_statement(stacknode* temp)
{
	if(strcmp(temp->ntortinfo->str, "<declareStmt>") == 0)
		return;
	if(strcmp(temp->ntortinfo->str, "<ioStmt>") == 0)
	{
	// 	mov rdi, scanint
	// lea rsi, [x]
	// mov al,0
	// call scanf
		if(strcmp(temp->child->ntortinfo->str, "GET_VALUE") == 0)
		{
			// idsymboltable* idst = temp->child->sibling->idst;
			// idsymboltablenode* pt = getidsymboltablenode(temp->child->sibling->tokinfo->lexeme, idst);

			printf("\tmov rdi, get_val\n");
			printf("\tlea rsi, [%s]\n", temp->child->sibling->tokinfo->lexeme);
			printf("\tmov al, 0\n");
			printf("\tcall scanf\n");
			printf("\n");
			return;
		}
		else if(strcmp(temp->child->ntortinfo->str, "PRINT") == 0)
		{
			stacknode* temp2 = temp->child->sibling->child;
			if(strcmp(temp2->ntortinfo->str, "NUM") == 0)
			{
				printf("\tmov rdi, print_val\n");
				printf("\tmov rsi, %s\n", temp2->tokinfo->lexeme);
				printf("\tmov al, 0\n");
				printf("\tcall printf\n");
				printf("\n");
			}
			else
			{
				// idsymboltable* idst = temp2->idst;
				// idsymboltablenode* pt = getidsymboltablenode(temp2->tokinfo->lexeme, idst);
				printf("\tmov rdi, print_val\n");
				printf("\tmov rax, [%s]\n", temp2->tokinfo->lexeme);
				printf("\tand rax, 00000000000000ffh\n");
				printf("\tmov rsi, rax\n");
				printf("\tmov al, 0\n");
				printf("\tcall printf\n");
				printf("\n");
			}
		}
	}

	return;
}

void traverseAST_forCodegen(stacknode* curr)
{
	if(curr == NULL)
		return;

	traverseAST_forCodegen(curr->child);

	if(strcmp(curr->ntortinfo->str, "<statements>") == 0)
	{
		stacknode* temp = curr->child;
		while(temp != NULL)
		{
			code_statement(temp);
			temp = temp->sibling;
		}
	}

	if(curr->child == NULL)
		return;
	
	stacknode* temp = curr->child;
	while(temp != NULL)
	{
		traverseAST_forCodegen(temp);
		temp = temp->sibling;
	}
	return;
}
/*
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
			printf("ERROR_T: type mismatch of %s and %s operands with %s operator\n", type1, type2, curr->ntortinfo->str);
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
			printf("ERROR_T: type mismatch of %s and %s with %s operator\n", type1, type2, curr->ntortinfo->str);
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
			printf("ERROR_T: type mismatch of %s and %s with logical %s\n", type1, type2, curr->ntortinfo->str);
			return error;
		}
	}

	return error;
}

*/
void generate_code(mainsymboltable* globalTable, stacknode* astroot)
{
	initialize(globalTable);
	traverseAST_forCodegen(astroot);
	endcode();
}