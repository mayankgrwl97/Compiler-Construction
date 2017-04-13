#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symboltable.h"
#include "hashtable.h"
#include "token.h"
#include "parserDef.h"
#include "mainsymboltable.h"
#include "idsymboltable.h"

idsymboltable* currsymboltable = NULL;
char* func_name;

void populatemainsymboltable(stacknode* curr, char* parent, mainsymboltable* globaltable)
{
	if(curr == NULL)
		return;
	populatemainsymboltable(curr->child, curr->ntortinfo->str, globaltable);

	if(curr->tokinfo != NULL && (strcmp(curr->tokinfo->tokenname, "PROGRAM") == 0))
	{
		if(!presentmainsymboltable(globaltable, "program"))
		{
			mainsymboltablenode* pt = insertmainsymboltable(globaltable, curr->tokinfo->lexeme);
			pt->idst = makeidsymboltable();
			func_name = "program";
			currsymboltable = pt->idst;
		}
		else
			printf("ERROR: program declared multiple times\n");
	}

	if(curr->tokinfo != NULL && (strcmp(curr->tokinfo->tokenname, "ID") == 0 ) && ((strcmp(parent, "<moduleDeclaration>") == 0) || (strcmp(parent, "<module>") == 0)))
	{
		// if(presentmainsymboltable(globaltable, "program"))
		// {
		// 	if(!presentmainsymboltable(globaltable, curr->tokinfo->lexeme))
		// 	{
		// 		printf("ERROR: %s not declared\n", curr->tokinfo->lexeme);
		// 		return;
		// 	}
		// }
		if(!presentmainsymboltable(globaltable, curr->tokinfo->lexeme))
		{
			mainsymboltablenode* pt = insertmainsymboltable(globaltable, curr->tokinfo->lexeme);
			pt->idst = makeidsymboltable();
			func_name = curr->tokinfo->lexeme;
			currsymboltable = pt->idst;
		}
	}

	if(curr->tokinfo != NULL && (strcmp(curr->tokinfo->tokenname, "ID") == 0) && (strcmp(parent, "<moduleReuseStmt>") == 0))
	{
		if(!presentmainsymboltable(globaltable, curr->tokinfo->lexeme))
		{
			printf("ERROR %s not declared\n", curr->tokinfo->lexeme);
		}
	}

	if(strcmp(curr->tokinfo->tokenname, "START") == 0)
	{
		idsymboltable* pt = makeidsymboltable();
		if(currsymboltable->child == NULL)
		{
			currsymboltable->child = pt;
			currsymboltable->child->parent = currsymboltable;
		}
		else
		{
			pt->sibling = currsymboltable->child;
			currsymboltable->child = pt;
			currsymboltable->child->parent = currsymboltable;
		}
		currsymboltable = currsymboltable->child;
	}
	if(strcmp(curr->tokinfo->tokenname, "END") == 0)
	{
		currsymboltable = currsymboltable->parent;
	}

	if(curr->tokinfo != NULL && strcmp(curr->tokinfo->tokenname, "ID") == 0)
	{
		insertidsymboltablenode(curr->tokinfo->lexeme, 0,0,currsymboltable);
	}

	if(curr->child == NULL)
		return;

	stacknode* temp = curr->child->sibling;
	while(temp != NULL)
	{
		populatemainsymboltable(temp, curr->ntortinfo->str, globaltable);
		temp = temp->sibling;
	}
	return;
}

int presentmainsymboltable(mainsymboltable* table, char* func_name)
{
	int h = hash(func_name);
	mainsymboltablenode* pt = table->buckets[h];

	while(pt != NULL)
	{
		if(strcmp(func_name,pt->func_name) == 0)
			return 1;
		pt = pt->next;
	}
	return 0;
}

mainsymboltablenode* insertmainsymboltable(mainsymboltable* table, char* func_name)
{
	int h = hash(func_name);
	mainsymboltablenode* pt = table->buckets[h];
	mainsymboltablenode* nd = makemainsymboltablenode(func_name);
	nd->next = pt;
	table->buckets[h] = nd;
	return nd;
}

mainsymboltablenode* makemainsymboltablenode(char* func_name)
{
	mainsymboltablenode* pt = (mainsymboltablenode*)malloc(sizeof(mainsymboltablenode));
	pt->func_name = func_name;
	pt->next = NULL;
	pt->idst = NULL;
}

void printmainsymboltable(mainsymboltable* table)
{
	for(int i=0; i<mainsymboltablesize; i++)
	{
		mainsymboltablenode* pt = table->buckets[i];
		printf("%d --> ", i);
		while(pt != NULL)
		{
			printf("%s | ", pt->func_name);
			pt = pt->next;
		}
		printf("\n");
	}
	return;
}