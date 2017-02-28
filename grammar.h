#ifndef _grammar_h_
#define _grammar_h_

#include "ntort.h"
#include "hashtable.h"

typedef struct node node;
struct node
{
	node* next;
	ntort* firstntort;
	ntort* lastntort;
};

extern node* grammar[60];
extern node toppointers[60];

node* makenode();
void populateGrammar(hashtable* table);
void printGrammar(hashtable* table);
void printTopDownGrammar(hashtable* table);

#endif