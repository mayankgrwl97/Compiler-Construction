/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "ntort.h"
#include "hashtable.h"
#include "grammar.h"

node* grammar[60];
node toppointers[60];
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
	insert(table, "$");

	for(int i=0; i<maxnonterminals; i++)
	{
		char lhsNT[100];
		fscanf(fp,"%s",buff); //scanning LHS nonterminal
		strcpy(lhsNT, buff);
		if(!present(table, buff))
			insert(table, buff);
		
		int ind = present(table, buff);

		fscanf(fp, "%s", buff); //scanning -->
		fscanf(fp, "%s", buff); //first RHS non terminal or terminal

		grammar[ind] = makenode();
		node* curr = grammar[ind];

		while(buff[0] != '$')
		{
			if(buff[0] == '|')
			{
				// insert $
				curr->lastntort->next = makentortnode(buff[0]=='<' ? 1 : 0, present(table,"$"), "$");
				curr->lastntort = curr->lastntort->next;
				curr->lastntort->next = makentortnode(1, present(table, lhsNT), lhsNT);
				curr->lastntort = curr->lastntort->next;
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
				if(buff[0] == '<')
				{
					if(toppointers[rhsind].firstntort == NULL)
					{
						toppointers[rhsind].firstntort = curr->lastntort;
						toppointers[rhsind].lastntort = curr->lastntort;
					}
					else
					{
						(toppointers[rhsind].lastntort)->down = curr->lastntort;
						toppointers[rhsind].lastntort = (toppointers[rhsind].lastntort)->down;
					}
				}
			}
			fscanf(fp, "%s", buff);
		}
		// insert $
		curr->lastntort->next = makentortnode(buff[0]=='<' ? 1 : 0, present(table,"$"), "$");
		curr->lastntort = curr->lastntort->next;
		curr->lastntort->next = makentortnode(1, present(table, lhsNT), lhsNT);
		curr->lastntort = curr->lastntort->next;
	}
	// printhashtable(table);
	fclose(fp);
	return;
}

void printTopDownGrammar(hashtable* table) //printing grammar down pointers
{
	FILE* fp = fopen("nonterminals.txt", "rb");
	for(int i=0; i<maxnonterminals; i++)
	{
		fscanf(fp,"%s",buff);
		int ind = present(table, buff);

		printf("%s --> ", buff);

		if(ind == 0)
		{
			printf("########ERROR######### %s\n", buff);
			return;
		}

		node curr = toppointers[ind];
		ntort* first = curr.firstntort;
		while(first != NULL)
		{
			printf(" %s ",first->next->str);
			first = first->down;
		}

		printf("\n");
	}
	return ;
}

void printGrammar(hashtable* table) //printing grammar
{
	FILE* fp = fopen("nonterminals.txt", "rb");
	for(int i=0; i<maxnonterminals; i++)
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
