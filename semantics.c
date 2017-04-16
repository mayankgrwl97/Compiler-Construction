#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symboltable.h"
#include "hashtable.h"
#include "token.h"
#include "parserDef.h"
#include "mainsymboltable.h"
#include "idsymboltable.h"
#include "typeExtractor.h"

char* func_name;

void checkSemantics(stacknode* curr, mainsymboltable* globaltable)
{
	if(curr == NULL)
		return;

	if(strcmp(curr->ntortinfo->str, "<moduleReuseStmt>") == 0)
	{
		func_name = curr->child->sibling->tokinfo->lexeme;
		mainsymboltablenode* pt = presentmainsymboltable(globaltable, func_name);
		if(pt == NULL)
			printf("ERROR_M MODULE %s not known.\n", curr->child->sibling->tokinfo->lexeme);

		int numActualReturn = 0, numOptional = 0;

		if(pt->oplist != NULL)
		{
			stacknode* helper = pt->oplist->child;	// pointing to ID
			while(helper != NULL)
			{
				numActualReturn++;
				helper = helper->sibling;
			}
		}

		if(curr->child->child != NULL)
		{
			stacknode* helper = curr->child->child;		// pointing to ID
			while(helper != NULL)
			{
				numOptional++;
				helper = helper->sibling;
			}
		}

		if(numActualReturn != numOptional)
			printf("ERROR_M: Output parameters numbers mismatch\n");
		else
		{
			// both return numbers are same check for types
			if(pt->oplist != NULL)
			{
				stacknode* id1 = pt->oplist->child;
				stacknode* id2 = curr->child->child;
				while(id1 != NULL)
				{
					idsymboltablenode* temp1 = getidsymboltablenode(id1->tokinfo->lexeme, id1->idst);
					idsymboltablenode* temp2 = getidsymboltablenode(id2->tokinfo->lexeme, id2->idst);
					int type1 = gettype(temp1->type);
					int type2 = gettype(temp2->type);
					if(type1 != type2)
					{
						printf("ERROR_M: Output parameters types mismatch\n");
						return;
					}
					id1 = id1->sibling;
					id2 = id2->sibling;
				}
			}
		}
		
	}

	stacknode* temp = curr->child;
	while(temp != NULL)
	{
		checkSemantics(temp, globaltable);
		temp = temp->sibling;
	}
}