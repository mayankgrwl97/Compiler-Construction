#ifndef _first_h_
#define _first_h_

#include "ntort.h"
#include "hashtable.h"
#include "grammar.h"

extern ntort* firstSets[60];
int done[60];

int calculateFirst(int ind);
void populateFirstSets(hashtable* table);

#endif