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
#include "first.h"


int calculateFirst(int ind) //calculate first sets corresponding to index ind assigned by hashtable
{
	if(done[ind] == 1)//if firstSet[ind]is already populated
	{
		ntort* curr = firstSets[ind];
		while(curr != NULL)//check if eps is in firstSet[ind]
		{
			if(strcmp(curr->str, "eps") == 0)
				return 1;
			curr = curr->next;
		}
		return 0;
	}
	ntort* last;
	node* currnode = grammar[ind];
	while(currnode != NULL)//traversing all nodes of rule corresponding to grammar[ind]
	{
		ntort* currntort = currnode->firstntort;//first ntort of currnode
		if(currntort->nt == 0)//if first ntort of currnode is terminal 
		{
			//add terminal directly and move to next node
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
		else//if first ntort of currnode is non terminal
		{
			int eps = calculateFirst(currntort->val);//populate firstSet for first non terminal
			ntort* firsthelper = firstSets[currntort->val];//pointer to firstSet of first NT

			//add firstSet from first NT except eps
			if(firstSets[ind] == NULL)
			{
				if(strcmp(firsthelper->str, "eps") != 0)//don't add eps from firstSet[NT] now
				{
					firstSets[ind] = makentortnode(0, firsthelper->val, firsthelper->str);
					firsthelper = firsthelper->next;
					last = firstSets[ind];
				}
				else
				{
					firsthelper = firsthelper->next;
				}
			}

			while(firsthelper != NULL)
			{
				if(strcmp(firsthelper->str, "eps") != 0)
				{
					last->next = makentortnode(0, firsthelper->val,firsthelper->str);
					firsthelper = firsthelper->next;
					last = last->next;
				}
				else
				{
					firsthelper = firsthelper->next;
				}
			}//added all elements of firstSet of first NT except eps

			while(eps)
			{	
				currntort = currntort->next;
				if(strcmp(currntort->str, "$") == 0)
				{
					if(firstSets[ind] == NULL)
					{
						firstSets[ind] = makentortnode(0, 190, "eps");
						firsthelper = firsthelper->next;
						last = firstSets[ind];
					}
					else
					{
						last->next = makentortnode(0, 190, "eps");
						firsthelper = firsthelper->next;
						last = last->next;
					}
					break;
				}
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
					break;
				}
				eps = calculateFirst(currntort->val);
				firsthelper = firstSets[currntort->val];

				if(firstSets[ind] == NULL)
				{
					if(strcmp(firsthelper->str, "eps") != 0)
					{
						firstSets[ind] = makentortnode(0, firsthelper->val, firsthelper->str);
						firsthelper = firsthelper->next;
						last = firstSets[ind];
					}
					else
					{
						firsthelper = firsthelper->next;
					}
				}

				while(firsthelper != NULL)
				{
					if(strcmp(firsthelper->str, "eps") != 0)
					{
						last->next = makentortnode(0, firsthelper->val,firsthelper->str);
						firsthelper = firsthelper->next;
						last = last->next;
					}
					else
					{
						firsthelper = firsthelper->next;
					}
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

void populateFirstSets(hashtable* table)
{
	FILE* fp = fopen("nonterminals.txt", "r");
	for(int i = 0;i < maxnonterminals; i++){
		// if(grammar[present(i)] == NULL)
		// 	return 0;
		char buff[100];
		fscanf(fp, "%s",buff);
		// printf("%s --> ", buff);
		calculateFirst(present(table, buff));
		// ntort* temp = firstSets[present(table, buff)];
		// while(temp != NULL){
		// 	printf("%s ",temp->str);
		// 	temp = temp->next;
		// }
		// printf("\n");
	}
	fclose(fp);
	return;
}

void printFirstSets(hashtable* table)
{
	FILE* fp = fopen("nonterminals.txt", "r");
	for(int i = 0; i<maxnonterminals; i++)
	{
		char buff[100];
		fscanf(fp, "%s",buff);
		printf("%s --> ", buff);
		ntort* temp = firstSets[present(table, buff)];
		while(temp != NULL)
		{
			printf("%s ",temp->str);
			temp = temp->next;
		}
		printf("\n");
	}
	return;
}