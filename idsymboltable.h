#ifndef _idsymboltable_h_
#define _idsymboltable_h_

#include "symboltable.h"
#include "hashtable.h"
#include "token.h"
#include "stack.h"

#define idsymboltablesize 100

enum types {integer = 0, real = 1, boolean = 2, error = 3};

typedef struct idsymboltablenode idsymboltablenode;
typedef struct stacknode stacknode;

struct idsymboltablenode
{
	char* idlex;
	stacknode* type;
	int offset;
	idsymboltablenode* next;
	int widthofid;
	int isAssigned;
};

typedef struct idsymboltable idsymboltable;

struct idsymboltable
{
	idsymboltablenode* buckets[idsymboltablesize];
	int offset;
	int startline;
	int endline;
	int nestinglevel;
	idsymboltable* child;
	idsymboltable* sibling;
	idsymboltable* parent;
	char* func_name;
};

idsymboltable* makeidsymboltable();
idsymboltablenode* makeidsymboltablenode(char* idlex, stacknode* type, int offset);
idsymboltablenode* insertidsymboltablenode(char* idlex, stacknode* type, int offset, idsymboltable* idst);
idsymboltablenode* getidsymboltablenode(char* idlex, idsymboltable* idst);
void printidsymboltable(idsymboltable* idst);
void printFunctionTable(idsymboltable* idst);
#endif