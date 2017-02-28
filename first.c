#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "ntort.h"
#include "hashtable.h"
#include "grammar.h"

ntort* firstSets[60];
int done[60];
// int terminaldone[60][200];

int calculateFirst(int ind)
{
	// traverse rule of this NT if it derives to eps then make next = 1
	if(done[ind] == 1)
	{
		ntort* curr = firstSets[ind];
		while(curr != NULL)
		{
			if(strcmp(curr->str, "eps") == 0)
				return 1;
			curr = curr->next;
		}
		return 0;
	}
	ntort* last;
	node* currnode = grammar[ind];
	while(currnode != NULL)
	{
		ntort* currntort = currnode->firstntort;
		if(currntort->nt == 0)
		{
			if(firstSets[ind] == NULL)
			{
				firstSets[ind] = makentortnode(0, currntort->val, currntort->str);
				last = firstSets[ind];
			}
			else
			{
				last->next = makentortnode(0, currntort->val, currntort->str);
				last = last->next;
			}
		}
		else
		{
			int eps = calculateFirst(currntort->val);
			ntort* firsthelper = firstSets[currntort->val];

			if(firstSets[ind] == NULL)
			{
				firstSets[ind] = makentortnode(0, firsthelper->val, firsthelper->str);
				firsthelper = firsthelper->next;
				last = firstSets[ind];
			}
			while(firsthelper != NULL)
			{
				last->next = makentortnode(0, firsthelper->val,firsthelper->str);
				firsthelper = firsthelper->next;
				last = last->next;
			}
			while(eps)
			{
				currntort = currntort->next;
				eps = calculateFirst(currntort->val);
				firsthelper = firstSets[currntort->val];

				if(firstSets[ind] == NULL)
				{
					firstSets[ind] = makentortnode(0, firsthelper->val, firsthelper->str);
					firsthelper = firsthelper->next;
					last = firstSets[ind];
				}
				while(firsthelper != NULL)
				{
					last->next = makentortnode(0, firsthelper->val,firsthelper->str);
					firsthelper = firsthelper->next;
					last = last->next;
				}
			}
		}

		currnode = currnode->next;
	}

	done[ind] = 1;
	ntort* curr = firstSets[ind];
	while(curr != NULL)
	{
		if(strcmp(curr->str, "eps") == 0)
			return 1;
		curr = curr->next;
	}
	return 0;
}

void populateFirstSets()
{
	for(int i=1; i<60; i++)
	{
		if(grammar[i] == NULL)
			break;

		
	}
	return;
}

int main()
{
	hashtable* table = makehashtable();
	populateGrammar(table);
	// printGrammar(table);
	FILE* fp = fopen("nonterminals.txt", "r");
	for(int i = 0;i < 51; i++){
		// if(grammar[present(i)] == NULL)
		// 	return 0;
		char buff[100];
		fscanf(fp, "%s",buff);
		printf("%s --> ", buff);
		calculateFirst(present(table, buff));
		ntort* temp = firstSets[present(table, buff)];
		while(temp != NULL){
			printf("%s ",temp->str);
			temp = temp->next;
		}
		printf("\n");
	}
	// calculateFirst(1);
	// // node* gra = grammar[1];
	// // 	printf("%s\n", gra->firstntort->str);
	// ntort* temp = firstSets[1];
	// while(temp != NULL){
	// 	printf("%s ",temp->str);
	// 	temp = temp->next;
	// }
	return 0;
}