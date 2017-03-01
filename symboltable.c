#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symboltable.h"
#include "hashtable.h"
#include "token.h"

symboltable* makesymboltable()
{
	symboltable* temp = (symboltable*)malloc(sizeof(symboltable));
	for(int i=0; i<symboltablesize; i++)
		temp->buckets[i] = NULL;
	return temp;
}

tokeninfo* presentsymboltable(symboltable* table, char* lexeme)
{
	int h = hash(lexeme);
	tokeninfo* pt = table->buckets[h];

	while(pt != NULL)
	{
		if(strcmp(lexeme,pt->lexeme) == 0)
			return pt;
		pt = pt->next;
	}
	return NULL;
}

void insertsymboltable(symboltable* table, char* tokenname, char* lexeme, int linenumber)
{
	// printf("%s\n",str);
	// static int ntval=1, tval=1;
	int h = hash(lexeme);
	tokeninfo* pt = table->buckets[h];
	tokeninfo* tok = makeToken(tokenname, lexeme, linenumber);
	tok->next = pt;
	table->buckets[h] = tok;
	return;
}

void printsymboltable(symboltable* table)
{
	for(int i=0; i<symboltablesize; i++)
	{
		tokeninfo* pt = table->buckets[i];
		printf("%d --> ", i);
		while(pt != NULL)
		{
			printf("lexeme=%s token=%s line=%d | ", pt->lexeme, pt->tokenname, pt->linenumber);
			pt = pt->next;
		}
		printf("\n");
	}
	return;
}