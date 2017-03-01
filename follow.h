#ifndef _follow_h_
#define _follow_h_

#include "ntort.h"
#include "hashtable.h"
#include "grammar.h"
#include "first.h"

ntort* followSets[60];
int done2[60];
void calculateFollow(int ind);
void populateFollowSets(hashtable* table);

#endif