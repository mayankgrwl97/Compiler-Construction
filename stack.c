#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "ntort.h"

void insertstack(stack* st, ntort* node)
{
	stacknode* newelem = (stacknode*)malloc(sizeof(stacknode));
	newelem->stackelem = makentortnode(node->nt, node->val, node->str);
	// ntort* newelem = makentortnode(node->nt, node->val, node->str);
	newelem->next = st->top;
	st->top = newelem;
	return;
}

ntort* popstack(stack* st)
{
	if(st->top == NULL)
		return NULL;
	ntort* topelem = st->top->stackelem;
	st->top = st->top->next;
	// topelem->next = NULL;
	return topelem;
}

ntort* topstack(stack* st)
{
	if(st->top == NULL)
		return NULL;
	return st->top->stackelem;
}

stack* makestack()
{
	stack* st = (stack*)malloc(sizeof(stack));
	st->top = NULL;
	return st;
}