/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

#ifndef _parserdef_h_
#define _parserdef_h_

#include "ntort.h"
#include "stack.h"
#include "token.h"
#include "lexerDef.h"
#include "grammar.h"

ntort* ParseTable[maxnonterminals+5][numberofterminals+5];
stacknode* root; //root node of parse tree

#endif