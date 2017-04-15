#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symboltable.h"
#include "hashtable.h"
#include "token.h"
#include "parserDef.h"
#include "mainsymboltable.h"
#include "idsymboltable.h"

int gettype(stacknode* type);
int typeofexpr(stacknode* curr);
void traverseAST_fortypechecking(stacknode* curr);