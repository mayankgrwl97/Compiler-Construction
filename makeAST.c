#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "makeAST.h"
#include "stack.h"
#include "ntort.h"

void makeAST(stacknode* curr, char* parent)
{
	if(curr == NULL)
		return;

	if(curr->sibling != NULL)
		makeAST(curr->sibling, parent);

	if(curr->child != NULL)
		makeAST(curr->child, curr->ntortinfo->str);

	if(curr->ntortinfo->nt == 0)
	{
		curr->nptr = curr;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<program>") == 0)
	{
		curr->nptr = curr;
		return;
	}

// ================================================================== //
	if(strcmp(curr->ntortinfo->str, "<moduleDeclarations>") == 0)
	{
		if(strcmp(parent, "<program>") == 0)
		{
			curr->nptr = curr;
			curr->child = curr->child->nptr;	// eps also handled
			return;
		}
		else if(strcmp(curr->child->ntortinfo->str, "eps") == 0)
		{
			curr->nptr = NULL;
			return;
		}
		else
		{
			curr->nptr = curr->child->nptr;
			return;
		}
	}

	if(strcmp(curr->ntortinfo->str, "<moduleDeclaration>") == 0)
	{
		curr->nptr = curr->child->sibling->sibling;
		curr->nptr->sibling = curr->sibling->nptr;
	}

// ================================================================== //

	if(strcmp(curr->ntortinfo->str, "<otherModules>") == 0)
	{
		if(strcmp(parent, "<program>") == 0)
		{
			curr->nptr = curr;
			curr->child = curr->child->nptr;
			return;
		}
		else if(strcmp(curr->child->ntortinfo->str, "eps") == 0)
		{
			curr->nptr = NULL;
			return;
		}
		else
		{
			curr->nptr = curr->child->nptr;
			return;
		}
	}

	if(strcmp(curr->ntortinfo->str, "<module>") == 0)
	{
		curr->nptr = curr;
		curr->child = curr->child->sibling->sibling; //for ID
		curr->child->sibling = curr->child->sibling->sibling->sibling->sibling->sibling->nptr; //for input_plist
		curr->child->sibling->sibling = curr->child->sibling->sibling->sibling->sibling->nptr; //for ret
		curr->child->sibling->sibling->sibling = curr->child->sibling->sibling->sibling->nptr; //for moduleDef
		curr->sibling = curr->sibling->nptr;
	}

// ================================================================== //

	if(strcmp(curr->ntortinfo->str, "<ret>") == 0)
	{
		if(strcmp(curr->child->ntortinfo->str, "eps") == 0)
		{
			// make empty output_plist node
			curr->child = (stacknode*)malloc(sizeof(stacknode));
			curr->child->tokinfo = curr->child->child = curr->child->sibling = curr->child->next = curr->child->nptr = NULL;
			curr->child->ntortinfo = makentortnode(1, 0, "<output_plist>");
			curr->nptr = curr->child;
		}
		else
		{
			curr->nptr = curr->child->sibling->sibling->nptr;
			curr->nptr->sibling = NULL;
		}
	}

	if(strcmp(curr->ntortinfo->str, "ARRAY") == 0)
	{
		curr->child = curr->sibling->sibling->nptr; //for range
		curr->child->sibling = curr->sibling->sibling->sibling->sibling->sibling->nptr;//for <type>
	}

	if(strcmp(curr->ntortinfo->str, "<dataType>") == 0)
	{
		curr->nptr = curr->child;
	}

	if(strcmp(curr->ntortinfo->str, "<N1>") == 0)
	{
		if(strcmp(curr->child->str, "eps") == 0)
		{
			curr->nptr = NULL;
			return;
		}
		curr->nptr = curr->child->sibling; //for ID
		curr->nptr->child = curr->nptr->sibling->sibling->nptr; //for dataType
		curr->nptr->sibling = curr->nptr->child->sibling->nptr; //for N1
		curr->nptr->child->sibling = NULL;
	}

	if(strcmp(curr->ntortinfo->str, "<input_plist>") == 0)
	{
		curr->nptr = curr;
		curr->child->child = curr->child->sibling->sibling->nptr;//for making dataType a child of ID
		curr->child->sibling = curr->child->child->sibling->nptr;//for making N1.nptr the sibling of ID
		curr->child->child->sibling = NULL;
	}

	if(strcmp(curr->ntortinfo->str, "<range>") == 0)
	{
		curr->nptr = curr;
		curr->child->sibling = curr->child->sibling->sibling;
	}

	if(strcmp(curr->ntortinfo->str, "<type>") == 0)
	{
		curr->nptr = curr->child;
	}

	if(strcmp(curr->ntortinfo->str, "<output_plist>") == 0)
	{
		curr->nptr = nptr;
		curr->child->child = curr->child->sibling->sibling->nptr;
		curr->child->sibling = curr->child->child->sibling->nptr;
		curr->child->child->sibling = NULL;
	}

	if(strcmp(curr->ntortinfo->str, "<N2>") == 0)
	{
		if(strcmp(curr->child->str, "eps") == 0)
		{
			curr->nptr = NULL;
			return;
		}
		curr->nptr = curr->child->sibling; //for ID
		curr->nptr->child = curr->nptr->sibling->sibling->nptr; //for type
		curr->nptr->sibling = curr->nptr->child->sibling->nptr; //for N2
		curr->nptr->child->sibling = NULL;
	}

// ================================================================== //

	if(strcmp(curr->ntortinfo->str, "<moduleDef>") == 0)
	{
		curr->nptr = curr;
		curr->child = curr->child->sibling->nptr;

	}

	return;
}

void printAST(stacknode* curr)
{
	if(curr == NULL)
		return;

	if(strcmp(curr->ntortinfo->str, "<moduleDeclarations>") == 0)
	{
		printf("%s\n", curr->ntortinfo->str);
		printf("Child is %s\n", curr->child->tokinfo->lexeme);
		stacknode* sib = curr->child->sibling;
		while(sib != NULL){
			printf("%s\n", sib->tokinfo->lexeme);
			sib = sib->sibling;
		}
		return;
	}
	// printAST(curr->child);

	return;
}