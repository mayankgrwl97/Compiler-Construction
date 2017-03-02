#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "grammar.h"
#include "hashtable.h"
#include "follow.h"
#include "first.h"
#include "ntort.h"
#include "stack.h"
#include "token.h"
#include "lexer.h"

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
	for(int i=0; i<58; i++)
		fscanf(fp2, "%s", term[i]);
	fclose(fp2);

	for(int i=0; i<51; i++)
	{
		char buff[50];
		fscanf(fp, "%s", buff);
		printf("%s \n", buff);
		int ind = present(table, buff);
		for(int j=0; j<58; j++)
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

stacknode* root;

void parseGrammar(hashtable* table, tokeninfo* lookahead)
{
	stack* st = makestack();
	insertstack(st, getnodehashtable(table, "$"), NULL);// insert $
	insertstack(st, getnodehashtable(table, "<program>"), NULL); //insert start symbol
	root = topstack(st);
	stacknode* topelem;
	// tokeninfo* lookahead = getAllTokens(testcasefile);

	// root = makentortnode(1, present(table,"<program>"), "<program>");

	while(strcmp(topstack(st)->ntortinfo->str, "$") != 0 && lookahead != NULL)
	{
		topelem = topstack(st);
		if(strcmp(topelem->ntortinfo->str, lookahead->tokenname) == 0)
		{
			topelem = popstack(st);
			topelem->tokinfo = lookahead;
			lookahead = lookahead->next;
			if(lookahead == NULL)
				return;
		}
		else if(strcmp(topelem->ntortinfo->str, "eps") == 0)
		{
			topelem = popstack(st);
		}
		else if(topelem->ntortinfo->nt == 0)
		{
			printf("ERROR1");
			return;
		}
		else if(ParseTable[topelem->ntortinfo->val][present(table, lookahead->tokenname)] == NULL)
		{
			printf("ERROR2");
			return;
		}
		else if(ParseTable[topelem->ntortinfo->val][present(table, lookahead->tokenname)] != NULL)
		{
			printf("%s -> ", topelem->ntortinfo->str);
			ntort* rule = ParseTable[topelem->ntortinfo->val][present(table, lookahead->tokenname)];
			// topelem->down = rule;
			stack* tempst = makestack();
			while(strcmp(rule->str, "$") != 0)
			{
				insertstack(tempst, rule, NULL);
				printf("%s ", rule->str);
				rule = rule->next;
			}
			topelem = popstack(st);
			while(topstack(tempst) != NULL)
			{
				insertstack(st, popstack(tempst)->ntortinfo, NULL);
				topstack(st)->sibling = topelem->child;
				topelem->child = topstack(st);
			}
			printf("\n");
		}
	}
	return;
}

void printParseTree(stacknode* curr, char* parent)
{
	// go on the first child itself then on remaining
	if(curr == NULL)
		return;
	printParseTree(curr->child, curr->ntortinfo->str);
	//itself

	if(curr->tokinfo != NULL)
		printf("%s    %d    %s    ", curr->tokinfo->lexeme, curr->tokinfo->linenumber,  curr->tokinfo->tokenname);
	else
		printf("----    ----    ----    ");

	if(curr->tokinfo != NULL && (strcmp(curr->tokinfo->tokenname,"NUM")==0 || strcmp(curr->tokinfo->tokenname,"RNUM")==0))
		printf("%s    ", curr->tokinfo->lexeme);
	else
		printf("----    ");

	printf("%s    %s    ", parent,(curr->ntortinfo->nt == 1 ? "no" : "yes"));

	if(curr->tokinfo == NULL)
		printf("%s\n", curr->ntortinfo->str);
	else
		printf("----\n");

	// printf("%s %s %s\n", curr->ntortinfo->str, parent, ());
	
	if(curr->child == NULL)
		return;
	stacknode* temp = curr->child->sibling;
	while(temp != NULL)
	{
		printParseTree(temp, curr->ntortinfo->str);
		temp = temp->sibling;
	}
	return;
}

int main()
{
	FILE* fp = fopen("testcase1.txt", "r");
	tokeninfo* tokens = getAllTokens(fp);
	// while(tokens != NULL)
	// {
	// 	printf("%s  %s  %d\n", tokens->tokenname, tokens->lexeme, tokens->linenumber);
	// 	tokens = tokens->next;
	// }

	hashtable* table = makehashtable();
	populateGrammar(table);
	populateFirstSets(table);
	populateFollowSets(table);
	makeParseTable(table);

	// tokeninfo* ptr = tokens;
	// while(ptr != NULL)
	// {
	// 	printf("%s ", ptr->tokenname);
	// 	ptr = ptr->next;
	// }
	parseGrammar(table, tokens);
	printf("\n\n\n");
	// printf("%s \n", root->str);
	// printf("%s \n", root->down->str);
	// printf("%s \n", root->down->next->str);
	printParseTree(root, "ROOT");
	fclose(fp);
	// printParseTable(table);
	// printGrammar(table);
	return 0;
}