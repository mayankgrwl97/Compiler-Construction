/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "follow.h"
#include "token.h"
#include "lexer.h"
#include "parserDef.h"

// ntort* ParseTable[maxnonterminals][numberofterminals];

void makeParseTable(hashtable* table)
{
	FILE* fp = fopen("nonterminals.txt", "rb");
	for(int i = 0; i <maxnonterminals; i++)
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
	char term[numberofterminals+5][20];

	FILE* fp = fopen("nonterminals.txt", "r");
	FILE* fp2 = fopen("terminals.txt","r");
	for(int i=0; i<numberofterminals; i++)
		fscanf(fp2, "%s", term[i]);
	fclose(fp2);

	for(int i=0; i<maxnonterminals; i++)
	{
		char buff[50];
		fscanf(fp, "%s", buff);
		printf("%s \n", buff);
		int ind = present(table, buff);
		for(int j=0; j<numberofterminals; j++)
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

int parseGrammar(hashtable* table, tokeninfo* lookahead)
{
	stack* st = makestack();
	insertstack(st, getnodehashtable(table, "$"), NULL);// insert $ on stack
	insertstack(st, getnodehashtable(table, "<program>"), NULL); //insert start symbol on stack

	root = topstack(st); //make root node of parse tree as topstack which is the start symbol
	stacknode* topelem;

	while(1)
	{
		topelem = topstack(st);
		if(strcmp(lookahead->tokenname, "$") == 0)
		{
			// printf("%s\n",topelem->ntortinfo->str);
			if(topelem != NULL && strcmp(topelem->ntortinfo->str, "$") == 0)//if $ reached and top of stack is $
			{
				lookahead = lookahead->next;
				popstack(st);
				return 1;
			}
			if(topelem != NULL && topelem->ntortinfo->nt == 0)//if $ reached and top of stack is terminal
			{
				if(strcmp(topelem->ntortinfo->str, "eps") == 0)
				{
					popstack(st);
					continue;
				}

				printf("Expected %s at line %d\n", topelem->ntortinfo->str, lookahead->linenumber);
				lookahead = lookahead->next;
				return 0;
			}
			if(topelem != NULL && topelem->ntortinfo->nt == 1)//if $ reached and top of stack is non terminal
			{

				int ind = topelem->ntortinfo->val;
				int tind = present(table, lookahead->tokenname);//tind for $

				if(ParseTable[ind][tind] != NULL)
				{
					ntort* rule = ParseTable[ind][tind];
					stack* tempst = makestack();

					while(strcmp(rule->str, "$") != 0)
					{
						insertstack(tempst, rule, NULL);
						rule = rule->next;
					}
					topelem = popstack(st);
					while(topstack(tempst) != NULL)
					{
						insertstack(st, popstack(tempst)->ntortinfo, NULL);
						topstack(st)->sibling = topelem->child;
						topelem->child = topstack(st);
					}
				}
				else
				{
					printf("Expected %s at line %d\n", (firstSets[ind])->str, lookahead->linenumber);
					return 0;
				}
			}
			// printf("Some other error\n");
			// return 0;
			// printf("Unexpected token %s found at line number ",topelem->ntortinfo->str);
		}

		else if(strcmp(topelem->ntortinfo->str, "$") == 0)//lookahead is something other than $ but top of stack is $
		{
			printf("Unexpected symbol %s at line %d\n", lookahead->lexeme, lookahead->linenumber);
			return 0;
		}

		else if(topelem->ntortinfo->nt == 0) //top of stack is terminal
		{
			// printf("%s\n", topelem->ntortinfo->str);
			if(strcmp(topelem->ntortinfo->str, "eps") == 0)
			{
				topelem = popstack(st);
				// printf("%s\n");
			}
			else if(strcmp(lookahead->tokenname, topelem->ntortinfo->str) == 0)
			{
				topelem = popstack(st);
				topelem->tokinfo = lookahead;
				lookahead = lookahead->next;
			}
			else
			{
				printf("Expected %s at line %d\n", topelem->ntortinfo->str, lookahead->linenumber);
				return 0;
			}
		}

		else if(topelem->ntortinfo->nt == 1) //top of stack is non terminal
		{
			int ind = topelem->ntortinfo->val;
			int tind = present(table, lookahead->tokenname);

			if(ParseTable[ind][tind] != NULL)
			{
				ntort* rule = ParseTable[ind][tind];
				stack* tempst = makestack();

				while(strcmp(rule->str, "$") != 0)
				{
					insertstack(tempst, rule, NULL);
					rule = rule->next;
				}
				topelem = popstack(st);
				while(topstack(tempst) != NULL)
				{
					insertstack(st, popstack(tempst)->ntortinfo, NULL);
					topstack(st)->sibling = topelem->child;
					topelem->child = topstack(st);
				}
			}
			else
			{
				printf("Expected %s at line %d\n", (firstSets[ind])->str, lookahead->linenumber);
				return 0;
			}
		}
	}
	return 0;
	// return (strcmp(st->top->ntortinfo->str, "$") == 0);
}

void printParseTree(stacknode* curr, char* parent, FILE* fp)
{
	// go on the first child itself then on remaining
	if(curr == NULL)
		return;
	printParseTree(curr->child, curr->ntortinfo->str, fp);
	//itself

	if(curr->tokinfo != NULL)
		fprintf(fp, "%-14s%-8d%-14s", curr->tokinfo->lexeme, curr->tokinfo->linenumber,  curr->tokinfo->tokenname);
	else
		fprintf(fp, "----          ----    ----          ");

	if(curr->tokinfo != NULL && (strcmp(curr->tokinfo->tokenname,"NUM")==0 || strcmp(curr->tokinfo->tokenname,"RNUM")==0))
		fprintf(fp, "%-8s", curr->tokinfo->lexeme);
	else
		fprintf(fp, "----    ");

	fprintf(fp, "%-27s%-8s", parent,(curr->ntortinfo->nt == 1 ? "no" : "yes"));

	if(curr->tokinfo == NULL)
		fprintf(fp, "%s\n", curr->ntortinfo->str);
	else
		fprintf(fp, "----\n");

	// printf("%s %s %s\n", curr->ntortinfo->str, parent, ());
	
	if(curr->child == NULL)
		return;
	stacknode* temp = curr->child->sibling;
	while(temp != NULL)
	{
		printParseTree(temp, curr->ntortinfo->str, fp);
		temp = temp->sibling;
	}
	return;
}