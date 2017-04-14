#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symboltable.h"
#include "hashtable.h"
#include "token.h"
#include "idsymboltable.h"
#include "stack.h"

idsymboltable* makeidsymboltable()
{
	idsymboltable* pt = (idsymboltable*)malloc(sizeof(idsymboltable));
	pt->child = pt->sibling = pt->parent = NULL;
	pt->offset = 0;
	for(int i=0; i<idsymboltablesize; i++)
		pt->buckets[i] = NULL;
	return pt;
}

idsymboltablenode* makeidsymboltablenode(char* idlex, stacknode* type, int offset)
{
	idsymboltablenode* pt = (idsymboltablenode*)malloc(sizeof(idsymboltablenode));
	pt->idlex = idlex;
	pt->type = type;
	pt->offset = offset;
	return pt;
}

void insertidsymboltablenode(char* idlex, stacknode* type, int offset, idsymboltable* idst)
{
	int h = hash(idlex);
	idsymboltablenode* pt = idst->buckets[h];
	idsymboltablenode* node = makeidsymboltablenode(idlex, type, offset);
	node->next = pt;
	idst->buckets[h] = node;
	return;
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

void printidsymboltable(idsymboltable* idst)
{
	for(int i = 0;i < idsymboltablesize; i++)
	{
		idsymboltablenode* temp = idst->buckets[i];
		while(temp != NULL)
		{
			printf("%s %s\t\n", temp->idlex, temp->type->ntortinfo->str);
			temp = temp->next;
		}
	}
}