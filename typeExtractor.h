#ifndef _typeExtractor_h_
#define _typeExtractor_h_

#include "mainsymboltable.h"

int gettype(stacknode* type);
int typeofexpr(stacknode* curr);
void traverseAST_fortypechecking(stacknode* curr);

#endif