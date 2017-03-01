#ifndef _symboltable_h_
#define _symboltable_h_

#include "hashtable.h"
#include "ntort.h"
#include "token.h"

#define symboltablesize 100

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