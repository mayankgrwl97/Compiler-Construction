/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

#ifndef _parser_h_
#define _parser_h_

#include "stack.h"

void makeParseTable(hashtable* table);
void printParseTable(hashtable* table);
int parseGrammar(hashtable* table, tokeninfo* lookahead, int toPrint);
void printParseTree(stacknode* curr, char* parent);
void parseTreeMemory(stacknode* curr, int* nodes, int* memory);

#endif