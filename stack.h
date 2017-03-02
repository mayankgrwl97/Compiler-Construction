#ifndef _stack_h_
#define _stack_h_

#include "ntort.h"

typedef struct stack stack;
struct stack
{
	ntort* top;
};

void insertstack(stack* st, ntort* node);
ntort* popstack(stack* st);
stack* makestack();
ntort* topstack(stack* st);

#endif