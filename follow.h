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

ntort* followSets[maxnonterminals];
int followdone[maxnonterminals];
int terminaladded[maxnonterminals][numberofterminals];
void calculateFollow(int ind);
void populateFollowSets(hashtable* table);
void printFollowSets(hashtable* table);

#endif