/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

#ifndef _symboltable_h_
#define _symboltable_h_

#include "hashtable.h"
#include "ntort.h"
#include "token.h"

#define symboltablesize 100


//Used for adding identifiers and keywords in lexer phase
typedef struct symboltable symboltable;
struct symboltable
{
	tokeninfo* buckets[symboltablesize];
};

void printsymboltable(symboltable* table);
symboltable* makesymboltable();
tokeninfo* presentsymboltable(symboltable* table, char* lexeme);
void insertsymboltable(symboltable* table, char* tokenname, char* lexeme, int linenumber);

#endif