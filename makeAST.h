#ifndef _makeAST_h_
#define _makeAST_h_

#include "parserDef.h"
#include "parser.h"


void makeAST(stacknode* curr, char* parent);
void printAST(stacknode* curr);

#endif