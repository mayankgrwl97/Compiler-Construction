/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

#ifndef _hashtable_h_
#define _hashtable_h_

#include "ntort.h"
#define hashtablesize 100

typedef struct hashtable hashtable;
struct hashtable
{
	ntort* buckets[hashtablesize];
};

int hash(char* str);
void printhashtable(hashtable* table);
hashtable* makehashtable();
int present(hashtable* table, char* str);
void insert(hashtable* table, char* str);
ntort* getnodehashtable(hashtable* table, char* str);

#endif