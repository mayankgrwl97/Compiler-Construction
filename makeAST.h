/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

#ifndef _makeAST_h_
#define _makeAST_h_

#include "parserDef.h"
#include "parser.h"


void makeAST(stacknode* curr, char* parent);
void printAST(stacknode* curr, char* parent);
void astMemory(stacknode* curr, int* nodes, int* memory);

#endif