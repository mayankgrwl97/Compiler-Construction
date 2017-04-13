#ifndef _idsymboltable_h_
#define _idsymboltable_h_

#include "symboltable.h"
#include "hashtable.h"
#include "token.h"

#define idsymboltablesize 100

enum types {integer = 0, real, boolean, array};

typedef struct idsymboltablenode idsymboltablenode;

struct idsymboltablenode
{
	char* idlex;
	int type;
	int offset;
	idsymboltablenode* next;
};

typedef struct idsymboltable idsymboltable;

struct idsymboltable
{
	idsymboltablenode* buckets[idsymboltablesize];
	int offset;
	idsymboltable* child;
	idsymboltable* sibling;
	idsymboltable* parent;
};

idsymboltable* makeidsymboltable();
idsymboltablenode* makeidsymboltablenode(char* idlex, int type, int offset);
void insertidsymboltablenode(char* idlex, int type, int offset, idsymboltable* idst);
idsymboltablenode* getidsymboltablenode(char* idlex, idsymboltable* idst);

#endif