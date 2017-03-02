/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "ntort.h"
#include "token.h"

void insertstack(stack* st, ntort* ntortinfo, tokeninfo* tokinfo)
{
	stacknode* newelem = (stacknode*)malloc(sizeof(stacknode));
	newelem->ntortinfo = makentortnode(ntortinfo->nt, ntortinfo->val, ntortinfo->str);
	newelem->tokinfo = tokinfo;
	// ntort* newelem = makentortnode(node->nt, node->val, node->str);
	newelem->next = st->top;
	newelem->child = NULL;
	newelem->sibling = NULL;

	st->top = newelem;
	return;
}

stacknode* popstack(stack* st)
{
	if(st->top == NULL)
		return NULL;
	stacknode* topelem = st->top;
	st->top = st->top->next;
	// topelem->next = NULL;
	return topelem;
}

stacknode* topstack(stack* st)
{
	if(st->top == NULL)
		return NULL;
	return st->top;
}

stack* makestack()
{
	stack* st = (stack*)malloc(sizeof(stack));
	st->top = NULL;
	return st;
}