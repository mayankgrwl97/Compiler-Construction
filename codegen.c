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

void initialize(/*FILE* fp, */mainsymboltable* globalTable)
{
	printf("section .bss\n");
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
				printf("\t%s\tresb %d\n", curr->idlex, curr->widthofid);
				curr = curr->next;
			}
		}
	}

	printf("\n");

	printf("section .txt\n");
	printf("\tglobal _start\n\n");

	printf("_start:\n");
	return;
}
/*
mov eax, 3
   mov ebx, 2
   mov ecx, num1  
   mov edx, 5          ;5 bytes (numeric, 1 for sign) of that information
   int 80h
*/

void codegenexp(stacknode* curr)
{
	if(curr == NULL)
		return;

	codegenexp(curr->child);

	if(curr->child == NULL)
		return;

	stacknode* temp = curr->child;
	while(temp != NULL)
	{
		codegenexp(temp);
		temp = temp->sibling;
	}

	// itself
	// if(strcmp(curr->))

}


void code_statement(stacknode* temp)
{
	if(strcmp(temp->ntortinfo->str, "<ioStmt>") == 0)
	{
		if(strcmp(temp->child->ntortinfo->str, "GET_VALUE") == 0)
		{
			printf("mov eax, 3\n");
			printf("mov ebx, 2\n");
			idsymboltable* idst = temp->child->sibling->idst;
			idsymboltablenode* pt = getidsymboltablenode(temp->child->sibling->tokinfo->lexeme, idst);
			printf("mov ecx, %s\n", pt->idlex);
			printf("mov edx, %d\n", pt->widthofid);
			printf("int 80h\n");
		}
	}
	if(strcmp(temp->ntortinfo->str, "<expression>") == 0)
	{
		codegenexp(temp);
	}
}

void traverseAST_forCodegen(stacknode* curr)
{
	if(curr == NULL)
		return;

	traverseAST_forCodegen(curr->child);

	if(strcmp(curr->ntortinfo->str, "<statements>") == 0)
		code_statement(curr->child);

	if(curr->child == NULL)
		return;
	
	stacknode* temp = curr->child;
	while(temp != NULL)
	{
		traverseAST_forCodegen(temp);
		temp = temp->sibling;
	}
}