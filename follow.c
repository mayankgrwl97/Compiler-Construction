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
#include "follow.h"
#include "lexerDef.h"

void calculateFollow(int ind)
{
	if(followdone[ind] == 1)
		return;

	ntort* start = toppointers[ind].firstntort;
	ntort* temp;
	ntort* followhelper;
	while(start != NULL)
	{
		temp = start;
		ntort* last;
		while(1)
		{
			if(strcmp(temp->next->str,"$") == 0)
			{
				//copy follow of temp->next->next to follow[ind]
				if(ind == temp->next->next->val)
					break;
				if(!followdone[temp->next->next->val])
					calculateFollow(temp->next->next->val);

				followhelper = followSets[temp->next->next->val];
				if(followSets[ind] == NULL)
				{
					followSets[ind] = makentortnode(0, followhelper->val, followhelper->str);
					last = followSets[ind];
					followhelper = followhelper->next;
				}
				while(followhelper != NULL)
				{
					last->next = makentortnode(0, followhelper->val, followhelper->str);
					last = last->next;
					followhelper = followhelper->next;
				}
				break;
			}
			else if(temp->next->nt == 0)
			{
				//append terminal to follow[ind]
				if(followSets[ind] == NULL)
				{
					followSets[ind] = makentortnode(0, temp->next->val, temp->next->str);
					last = followSets[ind];
				}
				else
				{
					last->next = makentortnode(0, temp->next->val, temp->next->str);
					last = last->next;
				}
				break;
			}
			else
			{
				followhelper = firstSets[temp->next->val];
				int containseps = 0;
				if(followSets[ind] == NULL)
				{
					if(strcmp(followhelper->str, "eps") == 0)
						containseps = 1;
					else
					{
						followSets[ind] = makentortnode(0, followhelper->val, followhelper->str);
						last = followSets[ind];
					}
					followhelper = followhelper->next;
				}
				while(followhelper != NULL)
				{
					if(strcmp(followhelper->str, "eps") == 0)
						containseps = 1;
					else
					{
						last->next = makentortnode(0, followhelper->val, followhelper->str);
						last = last->next;
					}
					followhelper = followhelper->next;
				}
				if(containseps)
					temp = temp->next;
				else
					break;
				//iterate and copy without eps
				//if eps present then see next
				//if no eps then break;
			}
		}
		start = start->down;
	}

	followdone[ind] = 1;
	return;
}

void populateFollowSets(hashtable* table)
{
	FILE* fp = fopen("nonterminals.txt", "rb");
	char buff[100];
	fscanf(fp, "%s", buff);
	int ind = present(table, buff);
	followSets[ind] = makentortnode(0, present(table,"$"), "$");
	followdone[ind] = 1;
	for(int i = 1;i < 51; i++)
	{
		fscanf(fp, "%s", buff);
		ind = present(table,buff);
		calculateFollow(ind);
	}
	fclose(fp);
	return;
}

void printFollowSets(hashtable* table)
{
	FILE* fp = fopen("nonterminals.txt", "r");

	char buff[100];

 	for(int i=0; i<maxnonterminals; i++)
	{
		fscanf(fp, "%s", buff);
		int ind = present(table, buff);
		printf("%s --> ", buff);
		ntort* temp = followSets[ind];
		while(temp != NULL)
		{
			printf(" %s ", temp->str);
			temp = temp->next;
		}
		printf("\n");
	}

	fclose(fp);
	return;
}

// int main()
// {
// 	hashtable* table = makehashtable();
// 	populateGrammar(table);
// 	populateFirstSets(table);

// 	FILE* fp = fopen("nonterminals.txt", "rb");
// 	char buff[100];
// 	fscanf(fp, "%s", buff);
// 	int ind = present(table,buff);
// 	followSets[ind] = makentortnode(0, 200, "$");
// 	followdone[ind] = 1;

// 	for(int i = 1;i < 51; i++)
// 	{
// 		fscanf(fp, "%s", buff);
// 		ind = present(table, buff);
// 		calculateFollow(ind);
// 		printf("%s --> ", buff);
// 		ntort* temp = followSets[ind];
// 		while(temp != NULL){
// 			printf(" %s ", temp->str);
// 			temp = temp->next;
// 		}
// 		printf("\n");
// 	}
// 	return 0;
// }