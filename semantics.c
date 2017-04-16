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

	if(strcmp(curr->ntortinfo->str, "<output_plist>") == 0)
	{
		stacknode* temp = curr->child;
		while(temp != NULL)
		{
			idsymboltablenode* helper = getidsymboltablenode(temp->tokinfo->lexeme, temp->idst);
			if(helper->isAssigned == 0)
				printf("ERROR_M: The output parameter %s does not get assigned a value\n", temp->tokinfo->lexeme);
			temp = temp->sibling;
		}
	}

	if(strcmp(curr->ntortinfo->str, "<moduleReuseStmt>") == 0)
	{
		func_name = curr->child->sibling->tokinfo->lexeme;
		mainsymboltablenode* pt = presentmainsymboltable(globaltable, func_name);
		if(pt == NULL){
			printf("ERROR_M: MODULE %s not known.\n", curr->child->sibling->tokinfo->lexeme);
			return;
		}

		if(curr->child->sibling->sibling->child->idst != NULL && strcmp(curr->child->sibling->sibling->child->idst->func_name, func_name) == 0)	
		{
			printf("ERROR_M: Recursion not allowed\n");
			return;
		}

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
					idsymboltablenode* temp1 = getidsymboltablenode(id1->tokinfo->lexeme, id1->idst);	// id1->idst could be null, check exception handling
					idsymboltablenode* temp2 = getidsymboltablenode(id2->tokinfo->lexeme, id2->idst);
					int type1 = gettype(temp1->type);
					int type2 = gettype(temp2->type);
					if(type1 != type2)
					{
						printf("ERROR_M: Output parameters types mismatch\n");
						break;
					}
					id1 = id1->sibling;
					id2 = id2->sibling;
				}
			}
		}
		
		int actualInput = 0, givenInput = 0;

		if(pt->iplist != NULL)
		{
			stacknode* helper = pt->iplist->child;	// pointing to ID
			while(helper != NULL)
			{
				actualInput++;
				helper = helper->sibling;
			}
		}

		stacknode* helper = curr->child->sibling->sibling->child;	// pointing to ID
		while(helper != NULL)
		{
			givenInput++;
			helper = helper->sibling;
		}

		if(actualInput != givenInput)
			printf("ERROR_M: Input parameters numbers mismatch\n");
		else
		{
			// check for types
			stacknode* id1 = pt->iplist->child;
			stacknode* id2 = curr->child->sibling->sibling->child;	// pointing to ID
			while(id1 != NULL)
			{
				idsymboltablenode* temp1 = getidsymboltablenode(id1->tokinfo->lexeme, id1->idst);	// id1->idst could be null, check exception handling
				idsymboltablenode* temp2 = getidsymboltablenode(id2->tokinfo->lexeme, id2->idst);
				int type1 = gettype(temp1->type);
				int type2 = gettype(temp2->type);
				if(type1 != type2)
				{
					printf("ERROR_M: Input parameters types mismatch\n");
					break;
				}
				id1 = id1->sibling;
				id2 = id2->sibling;
			}		
		}
	}

	// SEMANTICS FOR "SWITCH" STATEMENTS
	if(strcmp(curr->ntortinfo->str, "<condionalStmt>") == 0)
	{
		idsymboltablenode* pt = getidsymboltablenode(curr->child->tokinfo->lexeme, curr->child->idst);
		int type = gettype(pt->type);


		if(type == integer)
		{
			stacknode* temp = curr->child->sibling->sibling->child;
			while(temp != NULL)
			{
				if(strcmp(temp->child->ntortinfo->str, "NUM") != 0)
					printf("ERROR at line %d : integer case expected\n", temp->child->tokinfo->linenumber);

				temp = temp->sibling->sibling;
			}
			temp = curr->child->sibling->sibling->sibling;
			if(temp->child == NULL)
			{
				printf("ERROR : expected default case statement for integer type\n");
			}
		}
		else if(type == real)
		{
			printf("ERROR : switch case does not work with real identifiers\n");
		}
		else if(type == boolean)
		{
			stacknode* temp = curr->child->sibling->sibling->sibling; //for <default>
			if(temp->child != NULL)
				printf("ERROR : switch case with boolean id should not have default\n");

			temp = curr->child->sibling->sibling->child;
			int count = 0;
			while(temp != NULL)
			{
				count++;
				temp = temp->sibling->sibling;
			}

			if(count != 2)
			{
				printf("ERROR : switch case with boolean id cannot have more than two cases\n");
				return;
			}

			temp = curr->child->sibling->sibling->child;
			int tr=0, fl=0;
			while(temp != NULL)
			{
				if(strcmp(temp->child->ntortinfo->str, "TRUE") == 0)
					tr++;
				else if(strcmp(temp->child->ntortinfo->str, "FALSE") == 0)
					fl++;
				else
					printf("ERROR at line %d : switch case with boolean id cannot take NUM case value\n", temp->child->tokinfo->linenumber);
				temp = temp->sibling->sibling;
			}

			if(tr != 1 || fl != 1)
			{
				printf("ERROR : switch case with boolean id should have both TRUE and FALSE cases\n");
			}
		}
	}

	// SEMANTICS FOR "FOR" STATEMENTS
	if(strcmp(curr->ntortinfo->str, "<iterativeStmt>") == 0)
	{
		if(strcmp(curr->child->ntortinfo->str, "FOR") == 0)
		{
			char* tempid = curr->child->sibling->tokinfo->lexeme;
			stacknode* temp = curr->child->sibling->sibling->sibling->sibling->child;
			while(temp != NULL)
			{
				if(strcmp(temp->ntortinfo->str, "<assignmentStmt>") == 0)
				{
					if(strcmp(temp->child->tokinfo->lexeme, tempid) == 0 && strcmp(temp->child->sibling->ntortinfo->str, "<lvalueIDStmt>") == 0)
						printf("ERROR at line %d : cannot redefine %s\n", temp->child->tokinfo->linenumber, tempid);
				}
				temp = temp->sibling;
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