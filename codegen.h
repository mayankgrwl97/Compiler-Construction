/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

#ifndef _codegen_h_
#define _codegen_h_

#include "symboltable.h"
#include "hashtable.h"
#include "token.h"
#include "parserDef.h"
#include "mainsymboltable.h"
#include "idsymboltable.h"
#include "typeExtractor.h"

// void endcode();
// void initialize(/*FILE* fp, */mainsymboltable* globalTable);
// void codegenexp(stacknode* curr);
// int getlabel();
// void codegeniterative(stacknode* temp);
// void code_statement(stacknode* temp);
// void traverseAST_forCodegen(stacknode* curr);
// void generate_code(mainsymboltable* globalTable, stacknode* astroot);
void endcode(FILE* fp);
void initialize(FILE* fp, mainsymboltable* globalTable);
void codegenexp(FILE* fp, stacknode* curr);
int getlabel();
void codegeniterative(FILE* fp, stacknode* temp);
void codegenconditional(FILE* fp, stacknode* curr);
void code_statement(FILE* fp, stacknode* temp);
void traverseAST_forCodegen(FILE* fp, stacknode* curr);
void generate_code(FILE* fp, mainsymboltable* globalTable, stacknode* astroot);

#endif
