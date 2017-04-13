/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

#ifndef _stack_h_
#define _stack_h_

#include "ntort.h"
#include "token.h"

typedef struct stacknode stacknode;
struct stacknode
{
	ntort* ntortinfo;
	tokeninfo* tokinfo;
	stacknode* next;
	stacknode* child;
	stacknode* sibling;
	stacknode* nptr;
};

typedef struct stack stack;
struct stack
{
	stacknode* top;
};

void insertstack(stack* st, ntort* ntortinfo, tokeninfo* tokinfo);
stacknode* popstack(stack* st);
stacknode* topstack(stack* st);
stack* makestack();

#endif