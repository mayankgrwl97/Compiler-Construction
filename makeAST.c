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

	if(strcmp(curr->ntortinfo->str, "eps") == 0)
	{
		curr->nptr = NULL;
		return;
	}	// not stroing eps

	if(strcmp(curr->ntortinfo->str, "ARRAY") != 0 && curr->ntortinfo->nt == 0)
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
		}
		else if(strcmp(curr->child->ntortinfo->str, "eps") == 0)
			curr->nptr = NULL;
		else
			curr->nptr = curr->child->nptr;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<moduleDeclaration>") == 0)
	{
		curr->nptr = curr->child->sibling->sibling;
		curr->nptr->sibling = curr->sibling->nptr;	// making ID's sibling
		return;
	}

// ================================================================== //

	if(strcmp(curr->ntortinfo->str, "<otherModules>") == 0)
	{
		if(strcmp(parent, "<program>") == 0)
		{
			curr->nptr = curr;
			curr->child = curr->child->nptr;
		}
		else if(strcmp(curr->child->ntortinfo->str, "eps") == 0)
			curr->nptr = NULL;
		else
			curr->nptr = curr->child->nptr;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<module>") == 0)
	{
		curr->nptr = curr;
		curr->child = curr->child->sibling->sibling; //for ID
		curr->child->sibling = curr->child->sibling->sibling->sibling->sibling->sibling->nptr; //for input_plist
		stacknode* temp = curr->child->sibling->sibling->sibling->sibling->sibling->nptr;	// pointing to moduleDef
		curr->child->sibling->sibling = curr->child->sibling->sibling->sibling->sibling->nptr; //for ret
		curr->child->sibling->sibling->sibling = temp; //for moduleDef
		curr->sibling = curr->sibling->nptr;
		return;
	}

// ================================================================== //

	if(strcmp(curr->ntortinfo->str, "<ret>") == 0)
	{
		if(strcmp(curr->child->ntortinfo->str, "eps") == 0)
		{
			// make empty output_plist node
			curr->child = (stacknode*)malloc(sizeof(stacknode));
			curr->child->tokinfo = NULL;
			curr->child->child = curr->child->sibling = curr->child->next = curr->child->nptr = NULL;
			curr->child->ntortinfo = makentortnode(1, 0, "<output_plist>");
			curr->nptr = curr->child;
		}
		else
		{
			curr->nptr = curr->child->sibling->sibling->nptr;
			curr->nptr->sibling = NULL;
		}
		return;
	}

	if(strcmp(curr->ntortinfo->str, "ARRAY") == 0)
	{
		curr->nptr = curr;
		curr->child = curr->sibling->sibling->nptr; //for range
		curr->child->sibling = curr->sibling->sibling->sibling->sibling->sibling->nptr;//for <type>
		curr->sibling = NULL;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<dataType>") == 0)
	{
		curr->nptr = curr->child;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<N1>") == 0)
	{
		if(strcmp(curr->child->ntortinfo->str, "eps") == 0)
		{
			curr->nptr = NULL;
			return;
		}
		curr->nptr = curr->child->sibling; //for ID
		stacknode* temp = curr->nptr->sibling->sibling->sibling->nptr;	// temporary storage for N1
		curr->nptr->child = curr->nptr->sibling->sibling->nptr; // making dataType child of ID
		curr->nptr->sibling = temp; //for N1
		curr->nptr->child->sibling = NULL;	// making sibling of dataType NULL
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<input_plist>") == 0)
	{
		curr->nptr = curr;
		stacknode* temp = curr->child->sibling->sibling->sibling->nptr;
		curr->child->child = curr->child->sibling->sibling->nptr;//for making dataType a child of ID
		curr->child->sibling = temp;	//for making N1.nptr the sibling of ID
		curr->child->child->sibling = NULL;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<range>") == 0)
	{
		curr->nptr = curr;
		curr->child->sibling = curr->child->sibling->sibling;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<type>") == 0)
	{
		curr->nptr = curr->child;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<output_plist>") == 0)
	{
		curr->nptr = curr;
		stacknode* temp = curr->child->sibling->sibling->sibling->nptr;
		curr->child->child = curr->child->sibling->sibling->nptr;
		curr->child->sibling = temp;
		curr->child->child->sibling = NULL;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<N2>") == 0)
	{
		if(strcmp(curr->child->ntortinfo->str, "eps") == 0)
		{
			curr->nptr = NULL;
			return;
		}
		curr->nptr = curr->child->sibling; //for ID
		stacknode* temp = curr->nptr->sibling->sibling->sibling->nptr;
		curr->nptr->child = curr->nptr->sibling->sibling->nptr; //for type
		curr->nptr->sibling = temp; //for N2
		curr->nptr->child->sibling = NULL;
		return;
	}

// ================================================================== //

	if(strcmp(curr->ntortinfo->str, "<statement>") == 0)
	{
		curr->nptr = curr->child->nptr;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<ioStmt>") == 0)
	{
		if(strcmp(curr->child->ntortinfo->str, "GET_VALUE") == 0)
		{
			curr->nptr = curr;
			curr->child->sibling = curr->child->sibling->sibling;
			curr->child->sibling->sibling = NULL;
		}
		else
		{
			curr->nptr = curr;
			curr->child->sibling = curr->child->sibling->sibling->nptr;
			curr->child->sibling->sibling = NULL;
		}
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<var>") == 0)
	{
		curr->nptr = curr;
		return;
	}
	
	if(strcmp(curr->ntortinfo->str, "<whichId>") == 0)
	{
		curr->nptr = curr;
		if(strcmp(curr->child->ntortinfo->str, "eps") == 0)
		{
			curr->child = NULL;
			return;
		}
		curr->child = curr->child->sibling;
		curr->child->sibling = NULL;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<simpleStmt>") == 0)
	{
		curr->nptr = curr->child;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<whichStmt>") == 0)
	{
		curr->nptr = curr->child;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<index>") == 0)
	{
		curr->nptr = curr;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<moduleDef>") == 0)
	{
		curr->nptr = curr;
		curr->child->sibling = curr->child->sibling->nptr;
		// curr->child = curr->child->sibling->nptr;
		// curr->child->sibling = NULL;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<assignmentStmt>") == 0)
	{
		curr->nptr = curr;
		curr->child->sibling = curr->child->sibling->nptr;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<lvalueIDStmt>") == 0)
	{
		curr->nptr = curr;
		curr->child = curr->child->sibling->nptr;
		curr->child->sibling = NULL;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<lvalueARRStmt>") == 0)
	{
		curr->nptr = curr;
		curr->child = curr->child->sibling->nptr;
		curr->child->sibling = curr->child->sibling->sibling->sibling->nptr;
		curr->child->sibling->sibling = NULL;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<idList>") == 0)
	{
		curr->nptr = curr;
		curr->child->sibling = curr->child->sibling->nptr;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<N3>") == 0)
	{
		if(strcmp(curr->child->ntortinfo->str, "eps") == 0)
		{
			curr->nptr = NULL;
			return;
		}
		curr->nptr = curr->child->sibling; //for ID
		curr->nptr->sibling = curr->nptr->sibling->nptr; //for N3
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<optional>") == 0)
	{
		if(strcmp(curr->child->ntortinfo->str, "eps") == 0)
		{
			curr->child = (stacknode*)malloc(sizeof(stacknode));
			curr->child->tokinfo = NULL;
			curr->child->child = curr->child->sibling = curr->child->next = curr->child->nptr = NULL;
			curr->child->ntortinfo = makentortnode(1, 0, "<idList>");
			curr->nptr = curr->child;
			return;
		}
		curr->nptr = curr->child->sibling;
		curr->nptr->sibling = NULL;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<moduleReuseStmt>") == 0)
	{
		curr->nptr = curr;
		stacknode* temp = curr->child->sibling->sibling->sibling;
		stacknode* temp2 = curr->child->sibling->sibling->sibling->sibling->sibling->sibling->nptr;
		curr->child = curr->child->nptr;
		curr->child->sibling = temp;
		curr->child->sibling->sibling = temp2;
		curr->child->sibling->sibling->sibling = NULL;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<statements>") == 0)
	{
		if(strcmp(parent, "<statements>") != 0)
		{
			curr->nptr = curr;
			if(strcmp(curr->child->ntortinfo->str, "eps") == 0)
				curr->child = NULL;
			else
			{
				stacknode* temp = curr->child->sibling->nptr;
				curr->child = curr->child->nptr;
				curr->child->sibling = temp;
			}
		}
		else
		{
			if(strcmp(curr->child->ntortinfo->str, "eps") == 0)
				curr->child = NULL;
			else
			{
				stacknode* temp = curr->child->sibling->nptr;
				curr->child = curr->child->nptr;
				curr->child->sibling = temp;
			}
			curr->nptr = curr->child;
		}
		return;
	}

// ================================================================== //

	if(strcmp(curr->ntortinfo->str, "<relationalOp>") == 0)
	{
		curr->nptr = curr->child;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<logicalOp>") == 0)
	{
		curr->nptr = curr->child;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<op1>") == 0)
	{
		curr->nptr = curr->child;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<op2>") == 0)
	{
		curr->nptr = curr->child;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<level4>") == 0) //saving NUM, RNUM, <var>, <expression>, MINUS<expression>
	{
		// curr->sibling = curr->sibling->nptr;
		if(strcmp(curr->child->ntortinfo->str, "BO") == 0)
		{
			curr->nptr = curr->child->sibling->nptr;
			curr->nptr->sibling = NULL;
		}
		else if(strcmp(curr->child->ntortinfo->str, "MINUS") == 0)
		{
			curr->nptr = curr->child;
			curr->nptr->sibling = curr->nptr->sibling->sibling->nptr;
			curr->nptr->sibling->sibling = NULL;
		}
		else
		{
			curr->nptr = curr->child;
		}
		return;
	}

	if((strcmp(curr->ntortinfo->str, "<temp4>") == 0) || (strcmp(curr->ntortinfo->str, "<temp3>") == 0) || (strcmp(curr->ntortinfo->str, "<temp2>") == 0) || (strcmp(curr->ntortinfo->str, "<temp1>") == 0))
	{
		if(strcmp(curr->child->ntortinfo->str, "eps") == 0)
		{
			curr->nptr = NULL;
			return;
		}
		else
		{
			if(curr->child->sibling->sibling->nptr == NULL)
			{
				curr->nptr = curr->child->nptr;
				stacknode* temp = curr->child->sibling->nptr;
				curr->nptr->child = temp;
				// curr->nptr->sibling = NULL;
			}
			else
			{
				stacknode* temp = curr->child->sibling->sibling->nptr->child;
				stacknode* temp2 = curr->child->sibling->sibling->nptr;

				curr->child->sibling->sibling->nptr->child = curr->child->sibling->nptr;
				curr->child->sibling->sibling->nptr->child->sibling = temp;
				curr->nptr = curr->child->nptr;
				curr->nptr->child = temp2;
				// curr->nptr->sibling = NULL;
			}
			return;
		}
	}

	if((strcmp(curr->ntortinfo->str, "<level3>") == 0) || (strcmp(curr->ntortinfo->str, "<level2>") == 0) || (strcmp(curr->ntortinfo->str, "<level1>") == 0))
	{
		if(curr->child->sibling->nptr == NULL)
		{
			curr->nptr = curr->child->nptr;
			return;
		}
		else
		{
			stacknode* temp = curr->child->sibling->nptr->child;
			stacknode* temp2 = curr->child->sibling->nptr;

			temp2->child = curr->child->nptr;
			temp2->child->sibling = temp;
			curr->nptr = temp2;
			// curr->nptr->sibling = NULL;
			return;
		}
	}

	if(strcmp(curr->ntortinfo->str, "<expression>") == 0)
	{
		if(curr->child->sibling->nptr == NULL)
		{
			curr->nptr = curr;
			curr->child = curr->child->nptr;
			return;
		}
		else
		{
			stacknode* temp = curr->child->sibling->nptr->child;
			stacknode* temp2 = curr->child->sibling->nptr;

			temp2->child = curr->child->nptr;
			temp2->child->sibling = temp;
			curr->child = temp2;
			curr->nptr = curr;
			// curr->nptr->sibling = NULL;
			return;

		}
	}

// ================================================================== //

	if(strcmp(curr->ntortinfo->str, "<value>") == 0)
	{
		curr->nptr = curr;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<declareStmt>") == 0)
	{
		curr->nptr = curr;
		curr->child = curr->child->sibling->nptr;
		curr->child->sibling = curr->child->sibling->sibling->nptr;
		curr->child->sibling->sibling = NULL;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<condionalStmt>") == 0)
	{
		curr->nptr = curr;
		curr->child = curr->child->sibling->sibling;
		curr->child->sibling = curr->child->sibling->sibling;
		// curr->child->sibling = curr->child->sibling->sibling->sibling->nptr;
		// curr->child->sibling->sibling = curr->child->sibling->sibling->sibling->nptr;
		// curr->child->sibling->sibling->sibling = NULL;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<driverModule>") == 0)
	{
		curr->nptr = curr;
		curr->child = curr->child->sibling->sibling;
		curr->child->sibling = curr->child->sibling->sibling->nptr;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<caseStmts>") == 0)
	{
		curr->nptr = curr;
		curr->child = curr->child->sibling;
		stacknode* temp = curr->child->sibling->sibling->sibling->sibling->sibling->nptr;
		curr->child->sibling = curr->child->sibling->sibling->nptr;
		curr->child->sibling->sibling = temp;
		return;
	}

	if(strcmp(curr->ntortinfo->str, "<N9>") == 0)
	{
		if(strcmp(curr->child->ntortinfo->str, "eps") == 0)
			curr->nptr = NULL;
		else
		{
			curr->nptr = curr->child->sibling;
			stacknode* temp = curr->child->sibling->sibling->sibling->sibling->sibling->nptr;
			curr->nptr->sibling = curr->nptr->sibling->sibling->nptr;
			curr->nptr->sibling->sibling = temp;
		}
	}

	if(strcmp(curr->ntortinfo->str, "<default>") == 0)
	{
		if(strcmp(curr->child->ntortinfo->str, "eps") == 0)
		{
			curr->child = NULL;
		}
		else
		{
			curr->nptr = curr;
			curr->child = curr->child->sibling->sibling->nptr;
			curr->child->sibling = NULL;
		}
	}

	if(strcmp(curr->ntortinfo->str, "<iterativeStmt>") == 0)
	{
		curr->nptr = curr;
		if(strcmp(curr->child->ntortinfo->str, "FOR") == 0)
		{
			curr->child->sibling = curr->child->sibling->sibling; //FOR -> ID
			curr->child->sibling->sibling = curr->child->sibling->sibling->sibling->nptr;//ID -> <range>.nptr
			curr->child->sibling->sibling->sibling = curr->child->sibling->sibling->sibling->sibling;//<range>.nptr -> START
			// curr->child->sibling->sibling->sibling->sibling = NULL;
		}
		else
		{
			curr->child->sibling = curr->child->sibling->sibling->nptr; //WHILE -> <expression>.nptr
			curr->child->sibling->sibling = curr->child->sibling->sibling->sibling; //<expression>.nptr -> <statements>.nptr
			// curr->child->sibling->sibling->sibling = NULL;
		}
	}

// ================================================================== //

	return;
}

void printAST(stacknode* curr)
{
	if(curr == NULL)
		return;

	printf("%s\n", curr->ntortinfo->str);
	
	printAST(curr->child);

	if(curr->child == NULL)		return;
	stacknode* sib = curr->child->sibling;
	while(sib != NULL){
		printAST(sib);
		sib = sib->sibling;
	}
	return;
}