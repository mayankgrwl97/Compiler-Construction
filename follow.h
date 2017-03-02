/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

#ifndef _follow_h_
#define _follow_h_

#include "ntort.h"
#include "hashtable.h"
#include "grammar.h"
#include "first.h"
#include "lexerDef.h"

ntort* followSets[maxnonterminals+5];
int followdone[maxnonterminals+5];


void calculateFollow(int ind);
void populateFollowSets(hashtable* table);
void printFollowSets(hashtable* table);

#endif