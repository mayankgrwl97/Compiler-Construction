/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

#ifndef _grammar_h_
#define _grammar_h_

#include "ntort.h"
#include "hashtable.h"

#define maxnonterminals 51
//node stores the antecedent of a rule in the grammar
typedef struct node node;
struct node
{
	node* next;
	ntort* firstntort;//pointer to first ntort in node
	ntort* lastntort; //pointer to last ntort in node
};

node* grammar[maxnonterminals]; 
node toppointers[maxnonterminals];

node* makenode();
void populateGrammar(hashtable* table);
void printGrammar(hashtable* table);
void printTopDownGrammar(hashtable* table);

/*
Grammar stored as

<nonterminal> --> node -> node -> ... -> node
node : ntort -> ntort -> .... -> ntort
*/

#endif