#ifndef _mainsymboltable_h_
#define _mainsymboltable_h_

#include "hashtable.h"
#include "ntort.h"
#include "token.h"
#include "idsymboltable.h"

#define mainsymboltablesize 100

typedef struct mainsymboltablenode mainsymboltablenode;
struct mainsymboltablenode
{
	char* func_name;
	mainsymboltablenode* next;
	idsymboltable* idst; // pointer to identifiers symbol table
};

typedef struct mainsymboltable mainsymboltable;
struct mainsymboltable
{
	mainsymboltablenode* buckets[mainsymboltablesize];
};
void populatemainsymboltable(stacknode* curr, char* parent, mainsymboltable* globaltable);
int presentmainsymboltable(mainsymboltable* table, char* func_name);
mainsymboltablenode* insertmainsymboltable(mainsymboltable* table, char* func_name);
mainsymboltablenode* makemainsymboltablenode(char* func_name);
void printmainsymboltable(mainsymboltable* table);
#endif