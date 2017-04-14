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
			stacknode* temp2 = curr->child->sibling->sibling->sibling->child;
			while(temp2 != NULL)
			{
				populateStatements(temp2, newIdst);
				temp2 = temp2->sibling;
			}
		}
		else
		{
			// populateExpression()
			idsymboltable* newIdst = makeidsymboltable();
			newIdst->parent = currIdst;
			stacknode* temp = curr->child->sibling->sibling->child;
			while(temp != NULL)
			{
				populateStatements(temp, newIdst);
				temp = temp->sibling;
			}
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
		{
			stacknode* temp = curr->child->sibling->sibling->child->child;
			while(temp != NULL)
			{
				populateStatements(temp, currIdst);
				temp = temp->next;
			}
		}
	}

	else if(strcmp(curr->ntortinfo->str, "<moduleReuseStmt>") == 0)
	{
		if(curr->child->child != NULL)
		{
			stacknode* temp = curr->child->child;
			while(temp != NULL)
			{
				idsymboltable* temp2 = checkScope(currIdst, temp->tokinfo->lexeme);
				if(temp2 == NULL)
					printf("ERROR %s not declared in this scope\n", temp->tokinfo->lexeme);
				else
					temp->idst = temp2;

				temp = temp->sibling;
			}
		}

		// idsymboltable* temp = checkScope(currIdst, curr->child->sibling->tokinfo->lexeme);
		// if(temp == NULL)
		// 	printf("ERROR %s not declared in this scope\n", curr->child->sibling->tokinfo->lexeme);
		// else
		// 	curr->child->sibling->idst = temp;

		stacknode* temp2 = curr->child->sibling->sibling->child;
		while(temp2 != NULL)
		{
			idsymboltable* temp3 = checkScope(currIdst, temp2->tokinfo->lexeme);
			if(temp3 == NULL)
				printf("ERROR %s not declared in this scope\n", temp2->tokinfo->lexeme);
			else
				temp2->idst = temp3;
		}
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
		if(strcmp(func_name, "program") == 0)
		{
			pt->idst = makeidsymboltable();
		}
		stacknode* temp = curr->child->child;
		while(temp != NULL)
		{
			populateStatements(curr->child->child, pt->idst);		
			temp = temp->sibling;
		}
	}
	else if(strcmp(curr->ntortinfo->str, "<driverModule>") == 0)
	{
		insertmainsymboltable(globaltable, curr->child->tokinfo->lexeme);
	}
	else if(strcmp(curr->ntortinfo->str, "<input_plist>") == 0)
	{
		func_name = parent->child->tokinfo->lexeme;
		mainsymboltablenode* pt = presentmainsymboltable(globaltable, func_name);
		pt->idst = makeidsymboltable();
		stacknode* temp = curr->child;
		while(temp != NULL)
		{
			insertidsymboltablenode(temp->tokinfo->lexeme, temp->child, 0, pt->idst);	// offset not considered
			temp = temp->sibling;				
		}
	}
	else if(strcmp(curr->ntortinfo->str, "<output_plist>") == 0)
	{
		func_name = parent->child->tokinfo->lexeme;
		mainsymboltablenode* pt = presentmainsymboltable(globaltable, func_name);
		stacknode* temp = curr->child;
		while(temp != NULL)
		{
			insertidsymboltablenode(temp->tokinfo->lexeme, temp->child, 0, pt->idst);	// offset not considered
			temp = temp->sibling;				
		}
	}
	if(curr->child == NULL)
		return;
	stacknode* temp = curr->child->sibling;
	while(temp != NULL)
	{
		populatemainsymboltable(temp, curr, globaltable);
		temp = temp->sibling;
	}
	return;

}

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
		// printf("%d --> ", i);
		while(pt != NULL)
		{
			printf("Module Name -> %s\n", pt->func_name);
			idsymboltable* temp = pt->idst;
			printidsymboltable(temp);
			pt = pt->next;
		}
		// printf("\n");
	}
	return;
}

mainsymboltable* makemainsymboltable()
{
	mainsymboltable* pt = (mainsymboltable*) malloc(sizeof(mainsymboltable));
	for(int i=0; i<mainsymboltablesize; i++)
		pt->buckets[i] = NULL;
}