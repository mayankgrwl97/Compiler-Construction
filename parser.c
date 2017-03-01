#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "grammar.h"
#include "hashtable.h"
#include "follow.h"
#include "first.h"
#include "ntort.h"

#define maxterminals 500
#define maxnterminals 500

ntort* ParseTable[maxnterminals][maxterminals];

void makeParseTable(hashtable* table)
{
	FILE* fp = fopen("nonterminals.txt", "rb");
	for(int i = 0;i < 51; i++)
	{
		char buff[100];
		fscanf(fp, "%s", buff);
		int ind = present(table, buff);
		node* currnode = grammar[ind];

		while(currnode != NULL)
		{
			ntort* ptr = currnode->firstntort;
			if(strcmp(ptr->str, "eps") == 0)
			{
				ntort* followhelper = followSets[ind];
				while(followhelper != NULL)
				{
					int terminalind = followhelper->val;
					ParseTable[ind][terminalind] = currnode->firstntort;
					followhelper = followhelper->next;
				}
				currnode = currnode->next;
			}
			else
			{
				while(strcmp(ptr->str, "$") != 0)
				{
					if(ptr->nt == 0)
					{
						int terminalind = ptr->val;
						ParseTable[ind][terminalind] = currnode->firstntort;
						break;
					}

					int containseps = 0;
					ntort* firsthelper = firstSets[ptr->val];
					while(firsthelper != NULL)
					{
						if(strcmp(firsthelper->str, "eps") == 0)
						{
							firsthelper = firsthelper->next;
							containseps = 1;
						}
						else
						{
							int terminalind = firsthelper->val;
							ParseTable[ind][terminalind] = currnode->firstntort;
							firsthelper = firsthelper->next;
						}
					}
					if(containseps == 0)
						break;
					ptr = ptr->next;
				}
				if(strcmp(ptr->str, "$") == 0)
				{
					ntort* followhelper = followSets[ind];
					while(followhelper != NULL)
					{
						int terminalind = followhelper->val;
						ParseTable[ind][terminalind] = currnode->firstntort;
						followhelper = followhelper->next;
					}
				}

				currnode = currnode->next;
			}
		}
	}
	return ;
}


void printParseTable(hashtable* table)
{
	char term[60][20];
	FILE* fp = fopen("nonterminals.txt", "r");
	FILE* fp2 = fopen("terminals.txt","r");
	for(int i=0; i<54; i++)
		fscanf(fp2, "%s", term[i]);
	fclose(fp2);

	for(int i=0; i<51; i++)
	{
		char buff[50];
		fscanf(fp, "%s", buff);
		printf("%s \n", buff);
		int ind = present(table, buff);
		for(int j=0; j<54; j++)
		{
			int terminalind = present(table, term[j]);
			// printf("%d ", terminalind);
			if(ParseTable[ind][terminalind])
			{
				printf("%s -> ", term[j]);
				ntort* temp = ParseTable[ind][terminalind];
				while(strcmp(temp->str, "$") != 0)
				{
					printf("%s ", temp->str);
					temp = temp->next;
				}
				printf("\n");
			}
		}
		printf("\n");

	}
	return;
}

int main()
{
	hashtable* table = makehashtable();
	populateGrammar(table);
	populateFirstSets(table);
	populateFollowSets(table);
	makeParseTable(table);
	printParseTable(table);
	// printGrammar(table);
	return 0;
}