/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

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

	if(strcmp(curr->ntortinfo->str, "<var>") == 0)
	{
		if(curr->child == NULL || curr->child->idst == NULL)
			return;

		idsymboltablenode* helper = getidsymboltablenode(curr->child->tokinfo->lexeme, curr->child->idst);
		if(strcmp(helper->type->ntortinfo->str, "ARRAY") == 0)
		{
			if(curr->child->sibling->child == NULL)
			{
				printf("ERROR_V at line %d : Array should be used by some index. %s not indexed properly.\n", curr->child->tokinfo->linenumber, curr->child->tokinfo->lexeme);
			}
		}
		else
		{
			if(curr->child->sibling->child != NULL)
			{
				printf("ERROR_V at line %d : Identifier %s can not be used as an array.\n", curr->child->tokinfo->linenumber, curr->child->tokinfo->lexeme);
			}
		}
	}

	if(strcmp(curr->ntortinfo->str, "<assignmentStmt>") == 0)
	{
		if(curr->child == NULL || curr->child->idst == NULL)
			return;

		idsymboltablenode* helper = getidsymboltablenode(curr->child->tokinfo->lexeme, curr->child->idst);
		if(strcmp(helper->type->ntortinfo->str, "ARRAY") == 0)
		{
			if(strcmp(curr->child->sibling->ntortinfo->str, "<lvalueIDStmt>") == 0)
			{
				printf("ERROR_V at line %d : Array should be used by some index. %s not indexed properly.\n", curr->child->tokinfo->linenumber, curr->child->tokinfo->lexeme);
			}
		}
		else
		{
			if(strcmp(curr->child->sibling->ntortinfo->str, "<lvalueARRStmt>") == 0)
			{
				printf("ERROR_V at line %d : Identifier %s can not be used as an array.\n", curr->child->tokinfo->linenumber, curr->child->tokinfo->lexeme);
			}
		}
	}

	if(strcmp(curr->ntortinfo->str, "<output_plist>") == 0)
	{
		stacknode* temp = curr->child;
		while(temp != NULL)
		{
			if(temp->idst == NULL)
				return;
			else
			{
				idsymboltablenode* helper = getidsymboltablenode(temp->tokinfo->lexeme, temp->idst);
				if(helper->isAssigned == 0)
					printf("ERROR_M at line %d : In module %s, the output parameter %s does not get assigned a value.\n", temp->tokinfo->linenumber, temp->idst->func_name, temp->tokinfo->lexeme);
			}
			temp = temp->sibling;
		}
	}

	if(strcmp(curr->ntortinfo->str, "<moduleReuseStmt>") == 0)
	{
		func_name = curr->child->sibling->tokinfo->lexeme;	// invoked function
		mainsymboltablenode* pt = presentmainsymboltable(globaltable, func_name);
		if(pt == NULL || !pt->isdefined){
			printf("ERROR_M at line %d : Undefined reference to module %s.\n", curr->child->sibling->tokinfo->linenumber, curr->child->sibling->tokinfo->lexeme);
			return;
		}

		if(curr->child->sibling->sibling->child->idst != NULL && strcmp(curr->child->sibling->sibling->child->idst->func_name, func_name) == 0)	
		{
			printf("ERROR_M at line %d : In function %s, recursion not allowed.\n", curr->child->sibling->tokinfo->linenumber, curr->child->sibling->sibling->child->idst->func_name);
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
			printf("ERROR_M at line %d : Output parameters numbers mismatch.\n", curr->child->child->tokinfo->linenumber);
		else
		{
			// both return numbers are same check for types
			if(pt->oplist != NULL)
			{
				stacknode* id1 = pt->oplist->child;
				stacknode* id2 = curr->child->child;
				while(id1 != NULL)
				{
					if(id1->idst == NULL || id2->idst == NULL)
						return;
					idsymboltablenode* temp1 = getidsymboltablenode(id1->tokinfo->lexeme, id1->idst);	// id1->idst could be null, check exception handling
					idsymboltablenode* temp2 = getidsymboltablenode(id2->tokinfo->lexeme, id2->idst);
					int type1 = gettype(temp1->type);
					int type2 = gettype(temp2->type);
					if(type1 != type2)
					{
						printf("ERROR_M at line %d : Output parameters types mismatch.\n", curr->child->child->tokinfo->linenumber);
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
			printf("ERROR_M at line %d : Input parameters numbers mismatch.\n", curr->child->sibling->sibling->child->tokinfo->linenumber);
		else
		{
			// check for types
			stacknode* id1 = pt->iplist->child;
			stacknode* id2 = curr->child->sibling->sibling->child;	// pointing to ID
			while(id1 != NULL)
			{
				if(id1->idst == NULL || id2->idst == NULL)
					return;
				idsymboltablenode* temp1 = getidsymboltablenode(id1->tokinfo->lexeme, id1->idst);	// id1->idst could be null, check exception handling
				idsymboltablenode* temp2 = getidsymboltablenode(id2->tokinfo->lexeme, id2->idst);
				int type1 = gettype(temp1->type);
				int type2 = gettype(temp2->type);
				if(type1 != type2)
				{
					printf("ERROR_M at line %d : Input parameters types mismatch.\n", curr->child->sibling->sibling->child->tokinfo->linenumber);
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
		if(curr->child->idst == NULL)
			return;
		idsymboltablenode* pt = getidsymboltablenode(curr->child->tokinfo->lexeme, curr->child->idst);
		int type = gettype(pt->type);

		if(type == integer)
		{
			stacknode* temp = curr->child->sibling->sibling->child;
			while(temp != NULL)
			{
				if(strcmp(temp->child->ntortinfo->str, "NUM") != 0)
					printf("ERROR_S at line %d : Integer case expected.\n", temp->child->tokinfo->linenumber);

				temp = temp->sibling->sibling;
			}
			temp = curr->child->sibling->sibling->sibling;
			if(temp->child == NULL)
				printf("ERROR_S at line %d : Expected default case statement for integer type.\n", curr->child->sibling->sibling->sibling->sibling->tokinfo->linenumber);
		}
		else if(type == real)
		{
			printf("ERROR_S at line %d : Switch case does not work with real identifiers.\n", curr->child->tokinfo->linenumber);
		}
		else if(type == boolean)
		{
			stacknode* temp = curr->child->sibling->sibling->sibling; //for <default>
			if(temp->child != NULL)
				printf("ERROR_S at line %d : Switch case with boolean id should not have default.\n", temp->sibling->tokinfo->linenumber);

			temp = curr->child->sibling->sibling->child;
			int count = 0;
			while(temp != NULL)
			{
				count++;
				temp = temp->sibling->sibling;
			}

			if(count != 2)
			{
				printf("ERROR_S at line %d : Switch case with boolean id cannot have more than two cases.\n", curr->child->sibling->sibling->sibling->sibling->tokinfo->linenumber);
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
					printf("ERROR_S at line %d : Switch case with boolean id cannot take NUM case value.\n", temp->child->tokinfo->linenumber);
				temp = temp->sibling->sibling;
			}

			if(tr != 1 || fl != 1)
			{
				printf("ERROR_S at line %d : Switch case with boolean id should have both TRUE and FALSE cases.\n", curr->child->sibling->sibling->sibling->sibling->tokinfo->linenumber);
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
						printf("ERROR_V at line %d : Cannot redefine %s in for loop.\n", temp->child->tokinfo->linenumber, tempid);
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