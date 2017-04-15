/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexerDef.h"
#include "lexer.h"
#include "grammar.h"
#include "first.h"
#include "follow.h"
#include "parser.h"
#include "parserDef.h"
#include "mainsymboltable.h"
#include "makeAST.h"
#include "typeExtractor.h"

void printCleanFile()
{
	FILE* fp = fopen("cleanFile.txt", "r");
	fseek(fp,0,SEEK_END);
	size_t inputsize = ftell(fp);
	rewind(fp);

	for(int i=0; i<inputsize; i++)
	{
		char ch = fgetc(fp);
		printf("%c", ch);
	}

	return;
}

void printTokens()
{
	if(tokenlist == NULL)
		return;

	tokeninfo* temp = tokenlist;

	while(strcmp(temp->tokenname, "$") != 0)
	{
		if(strcmp(temp->tokenname, "ERROR_1") == 0)
		{
			printf("ERROR_1: Identifier at line %d is longer than the prescribed length\n", temp->linenumber);
		}
		else if(strcmp(temp->tokenname, "ERROR_2") == 0)
		{
			printf("ERROR_2: Unknown Symbol %s at line %d\n", temp->lexeme, temp->linenumber);
		}
		else if(strcmp(temp->tokenname, "ERROR_3") == 0)
		{
			printf("ERROR_3: Unknown pattern %s at line %d\n", temp->lexeme, temp->linenumber);
		}
		else{
			printf("%s %s %d\n", temp->tokenname, temp->lexeme, temp->linenumber);
		}
		temp = temp->next;
	}
	return;
}

int main(int argc, char* argv[])
{
	printf("(a) FIRST and FOLLOW set automated\n");
	printf("(b) Both lexical and syntax analysis modules implemented\n");
	printf("(c) Parse Tree also implemented\n");
	printf("(d) Error handling implemented\n\n");	
	if(argc < 3)
	{
		printf("USAGE: ./stage1exe testcase.txt parsetreeoutfile.txt");
		return 0;
	}
	FILE* fp = fopen(argv[1], "rb");
	removeComments(fp);
	fclose(fp);
	
	fp = fopen(argv[1], "rb");
	tokenlist = getAllTokens(fp);
	fclose(fp);

	hashtable* table = makehashtable();
	populateGrammar(table);

	// printhashtable(table);
	// printGrammar(table);
	// printTopDownGrammar(table);

	populateFirstSets(table);
	// printFirstSets(table);

	populateFollowSets(table);
	// printFollowSets(table);

	makeParseTable(table);
	// printParseTable(table);

	// int syntaxcorrect = parseGrammar(table, tokenlist);
	// printParseTree(root, "ROOT");

	
	int control;
	printf("------------------------------------------------------------\n");
	printf("Press the appropriate option\n");
	printf("1 : For removal of comments\n");
	printf("2 : For printing the token list generated by the lexer.\n");
	printf("3 : For parsing to verify the syntactic correctness of the input source code\n");
	printf("4 : For creating and printing parse tree\n");

	scanf("%d",&control);
	if(control == 1)
	{
		printCleanFile();
	}
	if(control == 2)
	{
		printTokens();
	}
	if(control == 3)
	{
		int syntaxcorrect = parseGrammar(table, tokenlist);
		if(syntaxcorrect)
			printf("Input source code is syntactically correct...........\n");
		else
			printf("ERROR in input source code parsing\n");
	}
	if(control == 4)
	{
		int syntaxcorrect = parseGrammar(table, tokenlist);
		if(syntaxcorrect)
		{
			FILE* parsetreefile = fopen(argv[2],"w+");
			printParseTree(root, "ROOT", parsetreefile);
			fclose(parsetreefile);
		}
		else
		{
			printf("ERROR in making Parse Tree because parsing is not correct\n");
		}
		mainsymboltable* globaltable = makemainsymboltable();

		makeAST(root, "ROOT");
		printAST(root);
		populatemainsymboltable(root, NULL, globaltable);
		// printmainsymboltable(globaltable);
		traverseAST_fortypechecking(root);
		// populatemainsymboltable(root, NULL, globaltable);
	}
	return 0;
}