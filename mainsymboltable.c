#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symboltable.h"
#include "hashtable.h"
#include "token.h"
#include "parserDef.h"
#include "mainsymboltable.h"
#include "idsymboltable.h"

idsymboltable* currsymboltable = NULL;
char* func_name;

idsymboltable* checkScope(idsymboltable* currIdst, char* idlex)
{
	if(currIdst == NULL)	
		return NULL;
	if(getidsymboltablenode(idlex, currIdst) != NULL)
		return currIdst;
	return checkScope(currIdst->parent, idlex);
}

void populateExpression(stacknode* curr, idsymboltable* currIdst)
{
	if(curr == NULL)
		return;

	populateExpression(curr->child, currIdst);

	// itself
	if(strcmp(curr->ntortinfo->str, "ID") == 0)
	{
		idsymboltable* temp = checkScope(currIdst, curr->tokinfo->lexeme);
		if(temp == NULL)
		{
			printf("ERROR %s not declared in this scope\n", curr->tokinfo->lexeme);
		}
		else
		{
			curr->idst = temp;
		}
	}
	stacknode* temp = curr->child->sibling;
	while(temp != NULL)
	{
		populateExpression(temp, currIdst);
		temp = temp->sibling;
	}
}

void populateStatements(stacknode* curr, idsymboltable* currIdst)
{
	if(curr == NULL)
		return;

	if(strcmp(curr->ntortinfo->str, "<ioStmt>") == 0)
	{
		if(strcmp(curr->child->ntortinfo->str, "GET_VALUE") == 0)
		{
			idsymboltable* temp = checkScope(currIdst, curr->child->sibling->tokinfo->lexeme);
			if(temp == NULL)
			{
				printf("ERROR %s not declared in this scope\n", curr->child->sibling->tokinfo->lexeme);
			}
			else
			{
				curr->child->sibling->idst = temp;	
			}
		}
		else
		{
			if(strcmp(curr->child->sibling->child->ntortinfo->str, "ID") == 0)
			{
				idsymboltable* temp = checkScope(currIdst, curr->child->sibling->child->tokinfo->lexeme);
				if(temp == NULL)
				{
					printf("ERROR %s not declared in this scope\n", curr->child->sibling->child->tokinfo->lexeme);
				}
				else
				{
					curr->child->sibling->child->idst = temp;
				}
				
				if(curr->child->sibling->child->sibling->child != NULL) // whichId
				{
					idsymboltable* temp2 = checkScope(currIdst, curr->child->sibling->child->sibling->child->tokinfo->lexeme);
					if(temp2 == NULL)
					{
						printf("ERROR %s not declared in scope\n", curr->child->sibling->child->sibling->child->tokinfo->lexeme);
					}
					else
					{
						curr->child->sibling->child->sibling->child->idst = temp2;
					}
				}
			}
		}
	}
	else if(strcmp(curr->ntortinfo->str, "<declareStmt>") == 0)
	{
		stacknode* temp = curr->child->child;
		while(temp != NULL)
		{
			insertidsymboltablenode(temp->tokinfo->lexeme, curr->child->sibling, 0, currIdst);	//curr->child->sibling is type node // offset not considered
			temp->idst = currIdst;
			temp = temp->sibling;
		}
	}
	else if(strcmp(curr->ntortinfo->str, "<iterativeStmt>") == 0)
	{
		if(strcmp(curr->child->ntortinfo->str, "FOR") == 0)
		{
			idsymboltable* temp = checkScope(currIdst, curr->child->sibling->tokinfo->lexeme);
			if(temp == NULL)
			{
				printf("ERROR %s not declared in this scope\n", curr->child->sibling->tokinfo->lexeme);
			}
			else
			{
				curr->child->sibling->idst = temp;
			}
			idsymboltable* newIdst = makeidsymboltable();
			newIdst->parent = currIdst;
			populateStatements(curr->child->sibling->sibling->sibling->child, newIdst);
		}
		else
		{
			// populateExpression()
			idsymboltable* newIdst = makeidsymboltable();
			newIdst->parent = currIdst;
			populateStatements(curr->child->sibling->sibling->child, newIdst);
		}
	}
	else if(strcmp(curr->ntortinfo->str, "<assignmentStmt>") == 0)
	{
		idsymboltable* temp = checkScope(currIdst, curr->child->tokinfo->lexeme);
		if(temp == NULL)
			printf("ERROR %s not declared in this scope\n", curr->child->tokinfo->lexeme);
		else
			curr->child->idst = temp;

		if(strcmp(curr->child->sibling->ntortinfo->str, "<lvalueIDStmt>") == 0)
		{
			// populateExpression();
		}
		else
		{
			if(strcmp(curr->child->sibling->child->child->ntortinfo->str, "ID") == 0)
			{
				idsymboltable* temp = checkScope(currIdst, curr->child->sibling->child->child->tokinfo->lexeme);
				if(temp == NULL)
					printf("ERROR %s not declared in this scope\n", curr->child->sibling->child->child->tokinfo->lexeme);
				else
					curr->child->sibling->child->child->idst = temp;
			}
			// populateExpression();
		}
	}
	else if(strcmp(curr->ntortinfo->str, "<condionalStmt>") == 0)
	{
		idsymboltable* temp = checkScope(currIdst, curr->child->tokinfo->lexeme);
		if(temp == NULL)
			printf("ERROR %s not declared in this scope\n", curr->child->tokinfo->lexeme);
		else
			curr->child->idst = temp;

		idsymboltable* newIdst = makeidsymboltable();
		newIdst->parent = currIdst;

		stacknode* temp2 = curr->child->sibling->child->sibling;//<caseStmt>
		while(temp2 != NULL)
		{
			populateStatements(temp2->child, newIdst);
			temp2 = temp2->sibling->sibling;
		}

		if(curr->child->sibling->sibling->child != NULL)
			populateStatements(curr->child->sibling->sibling->child->child, currIdst);
	}

	else if(strcmp(curr->ntortinfo->str, "<moduleReuseStmt>") == 0)
	{
		
	}
}

void populatemainsymboltable(stacknode* curr, stacknode* parent, mainsymboltable* globaltable)
{
	if(curr == NULL)
		return;
	populatemainsymboltable(curr->child, curr, globaltable);

	if(strcmp(curr->ntortinfo->str, "<moduleDeclarations>") == 0)
	{
		stacknode* temp = curr->child;
		while(temp != NULL)
		{
			insertmainsymboltable(globaltable, temp->tokinfo->lexeme);
			temp = temp->sibling;
		}
	}
	else if(strcmp(curr->ntortinfo->str, "<module>") == 0)
	{
		func_name = curr->child->tokinfo->lexeme;
		mainsymboltablenode* pt = presentmainsymboltable(globaltable, func_name);
		if(pt == NULL)
			pt = insertmainsymboltable(globaltable, func_name);

		pt->isdefined = 1;
		pt->iplist = curr->child->sibling;
		pt->oplist = pt->iplist->sibling;
	}
	else if(strcmp(curr->ntortinfo->str, "<moduleDef>") == 0)
	{
		func_name = parent->child->tokinfo->lexeme;
		mainsymboltablenode* pt = presentmainsymboltable(globaltable, func_name);
		currsymboltable = pt->idst = makeidsymboltable();

		
	}

	stacknode* temp = curr->child->sibling;
	while(temp != NULL)
	{
		populatemainsymboltable(temp, curr, globaltable);
		temp = temp->sibling;
	}
	return;

}

// void populatemainsymboltable(stacknode* curr, char* parent, mainsymboltable* globaltable)
// {
// 	if(curr == NULL)
// 		return;
// 	populatemainsymboltable(curr->child, curr->ntortinfo->str, globaltable);

// 	if(curr->tokinfo != NULL && (strcmp(curr->tokinfo->tokenname, "PROGRAM") == 0))
// 	{
// 		if(!presentmainsymboltable(globaltable, "program"))
// 		{
// 			mainsymboltablenode* pt = insertmainsymboltable(globaltable, curr->tokinfo->lexeme);
// 			pt->idst = makeidsymboltable();
// 			func_name = "program";
// 			currsymboltable = pt->idst;
// 		}
// 		else
// 			printf("ERROR: program declared multiple times\n");
// 	}

// 	if(curr->tokinfo != NULL && (strcmp(curr->tokinfo->tokenname, "ID") == 0 ) && ((strcmp(parent, "<moduleDeclaration>") == 0) || (strcmp(parent, "<module>") == 0)))
// 	{
// 		// if(presentmainsymboltable(globaltable, "program"))
// 		// {
// 		// 	if(!presentmainsymboltable(globaltable, curr->tokinfo->lexeme))
// 		// 	{
// 		// 		printf("ERROR: %s not declared\n", curr->tokinfo->lexeme);
// 		// 		return;
// 		// 	}
// 		// }
// 		if(!presentmainsymboltable(globaltable, curr->tokinfo->lexeme))
// 		{
// 			mainsymboltablenode* pt = insertmainsymboltable(globaltable, curr->tokinfo->lexeme);
// 			pt->idst = makeidsymboltable();
// 			func_name = curr->tokinfo->lexeme;
// 			currsymboltable = pt->idst;
// 		}
// 	}

// 	if(curr->tokinfo != NULL && (strcmp(curr->tokinfo->tokenname, "ID") == 0) && (strcmp(parent, "<moduleReuseStmt>") == 0))
// 	{
// 		if(!presentmainsymboltable(globaltable, curr->tokinfo->lexeme))
// 		{
// 			printf("ERROR %s not declared\n", curr->tokinfo->lexeme);
// 		}
// 	}

// 	if(strcmp(curr->tokinfo->tokenname, "START") == 0)
// 	{
// 		idsymboltable* pt = makeidsymboltable();
// 		if(currsymboltable->child == NULL)
// 		{
// 			currsymboltable->child = pt;
// 			currsymboltable->child->parent = currsymboltable;
// 		}
// 		else
// 		{
// 			pt->sibling = currsymboltable->child;
// 			currsymboltable->child = pt;
// 			currsymboltable->child->parent = currsymboltable;
// 		}
// 		currsymboltable = currsymboltable->child;
// 	}
// 	if(strcmp(curr->tokinfo->tokenname, "END") == 0)
// 	{
// 		currsymboltable = currsymboltable->parent;
// 	}

// 	if(curr->tokinfo != NULL && strcmp(curr->tokinfo->tokenname, "ID") == 0)
// 	{
// 		insertidsymboltablenode(curr->tokinfo->lexeme, 0,0,currsymboltable);
// 	}

// 	if(curr->child == NULL)
// 		return;

// 	stacknode* temp = curr->child->sibling;
// 	while(temp != NULL)
// 	{
// 		populatemainsymboltable(temp, curr->ntortinfo->str, globaltable);
// 		temp = temp->sibling;
// 	}
// 	return;
// }

mainsymboltablenode* presentmainsymboltable(mainsymboltable* table, char* func_name)
{
	int h = hash(func_name);
	mainsymboltablenode* pt = table->buckets[h];

	while(pt != NULL)
	{
		if(strcmp(func_name,pt->func_name) == 0)
			return pt;
		pt = pt->next;
	}
	return NULL;
}

mainsymboltablenode* insertmainsymboltable(mainsymboltable* table, char* func_name)
{
	int h = hash(func_name);
	mainsymboltablenode* pt = table->buckets[h];
	mainsymboltablenode* nd = makemainsymboltablenode(func_name);
	nd->next = pt;
	table->buckets[h] = nd;
	return nd;
}

mainsymboltablenode* makemainsymboltablenode(char* func_name)
{
	mainsymboltablenode* pt = (mainsymboltablenode*)malloc(sizeof(mainsymboltablenode));
	pt->func_name = func_name;
	pt->next = NULL;
	pt->idst = NULL;
	pt->isdefined = pt->isused = 0;
	// pt->iplist = (inputlist*)malloc(sizeof(inputlist));
	// pt->oplist = (outputlist*)malloc(sizeof(outputlist));
	// pt->iplist->first = pt->iplist->last = pt->oplist->first = pt->oplist->last = NULL;
	pt->iplist = pt->oplist = NULL;
}

void printmainsymboltable(mainsymboltable* table)
{
	for(int i=0; i<mainsymboltablesize; i++)
	{
		mainsymboltablenode* pt = table->buckets[i];
		printf("%d --> ", i);
		while(pt != NULL)
		{
			printf("%s | ", pt->func_name);
			pt = pt->next;
		}
		printf("\n");
	}
	return;
}