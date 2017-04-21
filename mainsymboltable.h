/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

#ifndef _mainsymboltable_h_
#define _mainsymboltable_h_

#include "hashtable.h"
#include "ntort.h"
#include "token.h"
#include "idsymboltable.h"

#define mainsymboltablesize 100

int semanticCorrect;

typedef struct mainsymboltablenode mainsymboltablenode;
struct mainsymboltablenode
{
	char* func_name;
	mainsymboltablenode* next;
	stacknode* iplist;
	stacknode* oplist;
	idsymboltable* idst; // pointer to identifiers symbol table
	int isdefined; //function is defined or not
	int isused; // function is called or not, why have we made this
	int isdeclared; // function is declared or not
};

typedef struct mainsymboltable mainsymboltable;
struct mainsymboltable
{
	mainsymboltablenode* buckets[mainsymboltablesize];
};

void traverseAST_fortypechecking(stacknode* curr);
mainsymboltable* makemainsymboltable();
void populatemainsymboltable(stacknode* curr, stacknode* parent, mainsymboltable* globaltable, int toPrint);
mainsymboltablenode* presentmainsymboltable(mainsymboltable* table, char* func_name);
mainsymboltablenode* insertmainsymboltable(mainsymboltable* table, char* func_name);
mainsymboltablenode* makemainsymboltablenode(char* func_name);
void printmainsymboltable(mainsymboltable* globaltable);
int getarrayrange(stacknode* type);
#endif