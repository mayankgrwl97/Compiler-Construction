/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

#ifndef _first_h_
#define _first_h_

#include "ntort.h"
#include "hashtable.h"
#include "grammar.h"

ntort* firstSets[maxnonterminals+5];
int done[maxnonterminals+5];

int calculateFirst(int ind);
void populateFirstSets(hashtable* table);
void printFirstSets(hashtable* table);

#endif