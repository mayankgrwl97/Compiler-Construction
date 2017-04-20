#ifndef _codegen_h_
#define _codegen_h_

#include "symboltable.h"
#include "hashtable.h"
#include "token.h"
#include "parserDef.h"
#include "mainsymboltable.h"
#include "idsymboltable.h"
#include "typeExtractor.h"

void initialize(/*FILE* fp, */mainsymboltable* globalTable);
void generate_code(mainsymboltable* globalTable, stacknode* astroot);

#endif