#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "ntort.h"
#include "hashtable.h"
#include "grammar.h"

node* grammar[60];
node* toppointers[60];
char buff[500];

node* makenode()
{
	node* temp = (node*)malloc(sizeof(node));
	temp->next = NULL;
	temp->firstntort = NULL;
	temp->lastntort = NULL;
	return temp;
}

void populateGrammar(hashtable* table)
{
	FILE* fp = fopen("newGrammar.txt","rb");
	for(int i=0; i<51; i++)
	{
		fscanf(fp,"%s",buff); //scanning LHS nonterminal
		if(!present(table, buff))
			insert(table, buff);
		
		int ind = present(table, buff);

		fscanf(fp, "%s", buff); //scanning -->
		fscanf(fp, "%s", buff);

		grammar[ind] = makenode();
		node* curr = grammar[ind];

		while(buff[0] != '$')
		{
			if(buff[0] == '|')
			{
				curr->next = makenode();
				curr = curr->next;
			}
			else
			{
				if(!present(table, buff))
					insert(table, buff);
				int rhsind = present(table, buff);

				if(curr->firstntort == NULL)
				{
					curr->firstntort = makentortnode(buff[0]=='<' ? 1 : 0, rhsind, buff);
					curr->lastntort = curr->firstntort;
				}
				else
				{
					curr->lastntort->next = makentortnode(buff[0]=='<' ? 1 : 0, rhsind, buff);
					curr->lastntort = curr->lastntort->next;
				}
			}
			fscanf(fp, "%s", buff);
		}
	}
	// printhashtable(table);
	fclose(fp);
	return;
}

void printGrammar(hashtable* table)
{
	FILE* fp = fopen("nonterminals.txt", "rb");
	for(int i=0; i<51; i++)
	{
		fscanf(fp,"%s",buff);
		int ind = present(table, buff);

		printf("%s --> ", buff);

		if(ind == 0)
		{
			printf("########ERROR######### %s\n", buff);
			return;
		}
		node* curr = grammar[ind];
		while(curr != NULL)
		{
			ntort* first = curr->firstntort;
			while(first != NULL)
			{
				printf(" %s ",first->str);
				first = first->next;
			}
			printf("|");
			curr = curr->next;
		}
		printf("\n");
	}
	return ;
}
