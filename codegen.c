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
		else if(strcmp)
	}
}