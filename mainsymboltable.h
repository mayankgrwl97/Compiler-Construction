#ifndef _mainsymboltable_h_
#define _mainsymboltable_h_

#include "hashtable.h"
#include "ntort.h"
#include "token.h"
#include "idsymboltable.h"

#define mainsymboltablesize 100

// typedef struct inputlist inputlist;
// struct inputlist
// {
// 	stacknode* first; //use next field of stacknode* to maintain structure of linked list
// 	stacknode* last;
// };

// typedef struct outputlist outputlist;
// struct outputlist
// {
// 	stacknode* first;
// 	stacknode* last;
// };

typedef struct mainsymboltablenode mainsymboltablenode;
struct mainsymboltablenode
{
	char* func_name;
	mainsymboltablenode* next;
	stacknode* iplist;
	stacknode* oplist;
	idsymboltable* idst; // pointer to identifiers symbol table
	int isdefined; //function is defined or not
	int isused; //function is called or not
};

typedef struct mainsymboltable mainsymboltable;
struct mainsymboltable
{
	mainsymboltablenode* buckets[mainsymboltablesize];
};
void populatemainsymboltable(stacknode* curr, stacknode* parent, mainsymboltable* globaltable);
mainsymboltablenode* presentmainsymboltable(mainsymboltable* table, char* func_name);
mainsymboltablenode* insertmainsymboltable(mainsymboltable* table, char* func_name);
mainsymboltablenode* makemainsymboltablenode(char* func_name);
void printmainsymboltable(mainsymboltable* table);
#endif