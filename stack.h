#ifndef _stack_h_
#define _stack_h_

#include "ntort.h"

typedef struct stacknode stacknode;
struct stacknode
{
	ntort* stackelem;
	stacknode* next;
};

typedef struct stack stack;
struct stack
{
	stacknode* top;
};


void insertstack(stack* st, ntort* node);
ntort* popstack(stack* st);
stack* makestack();
ntort* topstack(stack* st);

#endif