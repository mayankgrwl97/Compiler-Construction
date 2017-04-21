/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

#ifndef _typeExtractor_h_
#define _typeExtractor_h_

#include "mainsymboltable.h"

int gettype(stacknode* type);
int typeofexpr(stacknode* curr);
void traverseAST_fortypechecking(stacknode* curr);

#endif