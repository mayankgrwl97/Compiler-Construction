#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symboltable.h"
#include "hashtable.h"
#include "token.h"
#include "idsymboltable.h"
#include "stack.h"
#include "mainsymboltable.h"

idsymboltable* makeidsymboltable()
{
	idsymboltable* pt = (idsymboltable*)malloc(sizeof(idsymboltable));
	pt->child = pt->sibling = pt->parent = NULL;
	pt->offset = 0;
	for(int i=0; i<idsymboltablesize; i++)
		pt->buckets[i] = NULL;
	pt->startline = pt->endline = 0;
	pt->nestinglevel = 0;
	return pt;
}

idsymboltablenode* makeidsymboltablenode(char* idlex, stacknode* type, int offset)
{
	idsymboltablenode* pt = (idsymboltablenode*)malloc(sizeof(idsymboltablenode));
	pt->idlex = idlex;
	pt->type = type;
	pt->offset = offset;
	pt->widthofid = 0;
	pt->isAssigned = 0;
	return pt;
}

idsymboltablenode* insertidsymboltablenode(char* idlex, stacknode* type, int offset, idsymboltable* idst)
{
	int h = hash(idlex);
	idsymboltablenode* pt = idst->buckets[h];
	idsymboltablenode* node = makeidsymboltablenode(idlex, type, offset);
	node->next = pt;
	idst->buckets[h] = node;
	return node;
}

idsymboltablenode* getidsymboltablenode(char* idlex, idsymboltable* idst)
{
	int h = hash(idlex);
	idsymboltablenode* pt = idst->buckets[h];
	while(pt != NULL)
	{
		if(strcmp(idlex, pt->idlex) == 0)
			return pt;
		pt = pt->next;
	}
	return NULL;
}

int serial_no = 1;

void printidsymboltable(idsymboltable* idst)
{
	for(int i = 0;i < idsymboltablesize; i++)
	{
		idsymboltablenode* temp = idst->buckets[i];
		while(temp != NULL)
		{
			char* printFuncName =  idst->func_name;
			if(strcmp(printFuncName, "program") == 0)
				printFuncName = "driver";	// because we are storing program instead of driver
			if(strcmp(temp->type->ntortinfo->str, "ARRAY") == 0)
			{
				char str[50];
				sprintf(str,"ARRAY(%d, %s)", getarrayrange(temp->type), temp->type->child->sibling->ntortinfo->str);
				printf("%d\t%-25s%-25s%-10s\t%d to %-5d\t%d\t%d\t%d\n", serial_no, temp->idlex, str, printFuncName, idst->startline, idst->endline, idst->nestinglevel, temp->widthofid, temp->offset);
			}
			else
				printf("%d\t%-25s%-25s%-10s\t%d to %-5d\t%d\t%d\t%d\n", serial_no, temp->idlex, temp->type->ntortinfo->str, printFuncName,idst->startline, idst->endline, idst->nestinglevel, temp->widthofid, temp->offset);
			serial_no++;
			temp = temp->next;
		}
	}
}

void printFunctionTable(idsymboltable* idst)
{
	printidsymboltable(idst);
	if(idst->child == NULL)
		return;
	printidsymboltable(idst->child);
	idsymboltable* helper = idst->child->sibling;
	while(helper != NULL)
	{
		printidsymboltable(helper);
		helper = helper->sibling;
	}
}