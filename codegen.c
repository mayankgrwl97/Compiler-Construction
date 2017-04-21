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
#include "codegen.h"

void endcode()
{
	printf("\tmov eax, 1\n");
	printf("\tmov ebx, 1\n");
	printf("\tint 80h\n");
	// printf("\tret\n");
	return;
}

void initialize(/*FILE* fp, */mainsymboltable* globalTable)
{
	printf("SECTION .bss\n");
	// fprintf(fp, "")
	mainsymboltablenode* temp = presentmainsymboltable(globalTable, "program");
	idsymboltable* pt = temp->idst;
	for(int i=0; i<idsymboltablesize; i++)
	{
		if(pt->buckets[i] != NULL)
		{
			idsymboltablenode* curr = pt->buckets[i];
			while(curr != NULL)
			{
				printf("%s\tresb %d\n", curr->idlex, 2*curr->widthofid);
				curr = curr->next;
			}
		}
	}

	printf("\n");

	printf("SECTION .data\n");
	printf("get_val: db \"%%d\", 0\n");
	printf("print_val: db \"%%d\", 10, 0\n\n");
	printf("SECTION .text\n\n");
	printf("extern printf\n");
	printf("extern scanf\n\n");
	printf("GLOBAL main\n\n");

	printf("main:\n");
	return;
}

void codegenexp(stacknode* curr)
{
	if(strcmp(curr->ntortinfo->str, "<expression>") == 0)
		codegenexp(curr->child);

	if(strcmp(curr->ntortinfo->str, "<var>") == 0)
	{
		if(strcmp(curr->child->ntortinfo->str, "NUM") == 0)
		{
			printf("\tmov r8, %s\n", curr->child->tokinfo->lexeme);
			printf("\tpush r8\n");
			return;
		}
		else if(strcmp(curr->child->ntortinfo->str, "RNUM") == 0)
		{
			// not to be considered
		}
		else
		{			
			idsymboltablenode* temp = getidsymboltablenode(curr->child->tokinfo->lexeme, curr->child->idst);
			printf("\tmov r8, [%s]\n", temp->idlex);
			printf("\tand r8, 00000000000000ffh\n");
			printf("\tpush r8\n");
		}
	}

	if(strcmp(curr->ntortinfo->str, "MINUS") == 0)
	{
		if(curr->sibling != NULL)
		{
			codegenexp(curr->sibling);
			printf("\tpop r9\n");
			printf("\tmov r8, 0\n");
			printf("\tsub r8, r9\n");
			printf("\tpush r8\n");
		}
		else
		{
			codegenexp(curr->child);
			codegenexp(curr->child->sibling);
			printf("\tpop r9\n");
			printf("\tpop r8\n");
			printf("\tsub r8,r9\n");
			printf("\tpush r8\n");
		}
	}

	if(strcmp(curr->ntortinfo->str, "PLUS") == 0)
	{
		codegenexp(curr->child);
		codegenexp(curr->child->sibling);
		printf("\tpop r9\n");
		printf("\tpop r8\n");
		printf("\tadd r8,r9\n");
		printf("\tpush r8\n");
	}

	if(strcmp(curr->ntortinfo->str, "MUL") == 0)
	{
		codegenexp(curr->child);
		codegenexp(curr->child->sibling);
		printf("\tpop r9\n");
		printf("\tpop r8\n");
		printf("\timul r8,r9\n");
		printf("\tpush r8\n");
	}

	if(strcmp(curr->ntortinfo->str, "DIV") == 0)
	{
		codegenexp(curr->child);
		codegenexp(curr->child->sibling);
		printf("\tpop r9\n");
		printf("\tpop r8\n");
		printf("\tdiv r8,r9\n");
		printf("\tpush r8\n");
	}

	if((strcmp(curr->ntortinfo->str, "LT") == 0) || (strcmp(curr->ntortinfo->str, "LE") == 0) || (strcmp(curr->ntortinfo->str, "GT") == 0) || (strcmp(curr->ntortinfo->str, "GE") == 0) || (strcmp(curr->ntortinfo->str, "EQ") == 0) || (strcmp(curr->ntortinfo->str, "NE") == 0))
	{
		codegenexp(curr->child);
		codegenexp(curr->child->sibling);
		int truelabel = getlabel();
		int endlabel = getlabel();
		printf("\tpop r9\n");
		printf("\tpop r8\n");
		printf("\tcmp r8,r9\n");
		if(strcmp(curr->ntortinfo->str, "LT") == 0)
			printf("\tjl truelabel_%d\n", truelabel);
		else if(strcmp(curr->ntortinfo->str, "LE") == 0)
			printf("\tjle truelabel_%d\n", truelabel);
		else if(strcmp(curr->ntortinfo->str, "GT") == 0)
			printf("\tjg truelabel_%d\n", truelabel);
		else if(strcmp(curr->ntortinfo->str, "GE") == 0)
			printf("\tjge truelabel_%d\n", truelabel);
		else if(strcmp(curr->ntortinfo->str, "EQ") == 0)
			printf("\tje truelabel_%d\n", truelabel);
		else if(strcmp(curr->ntortinfo->str, "NEQ") == 0)
			printf("\tjne truelabel_%d\n", truelabel);
		printf("\tmov r8, 0\n");
		printf("\tpush r8\n");
		printf("\tjmp label_%d\n", endlabel);
		printf("truelabel_%d:\n", truelabel);
		printf("\tmov r8, 1\n");
		printf("\tpush r8\n");
		printf("label_%d:\n", endlabel);
	}

	if(strcmp(curr->ntortinfo->str, "AND") == 0)
	{
		codegenexp(curr->child);
		codegenexp(curr->child->sibling);
		printf("\tpop r9\n");
		printf("\tpop r8\n");
		int firsttruelabel = getlabel();
		int secondfalselabel = getlabel();
		int endlabel = getlabel();
		printf("\tcmp r8, 0000000000000001h\n");
		printf("\tje label_%d\n", firsttruelabel);
		printf("\tmov r8, 0000000000000000h\n");
		printf("\tpush r8\n");
		printf("\tjmp label_%d\n", endlabel);
		printf("label_%d:\n", firsttruelabel);
		printf("\tcmp r9, 0000000000000001h\n");
		printf("\tjne label_%d\n", secondfalselabel);
		printf("\tmov r8, 0000000000000001h\n");
		printf("\tpush r8\n");
		printf("\tjmp label_%d\n", endlabel);
		printf("label_%d:\n",secondfalselabel);
		printf("\tmov r8, 0000000000000000h\n");
		printf("\tpush r8\n");
		printf("label_%d:\n",endlabel);
	}

	if(strcmp(curr->ntortinfo->str, "OR") == 0)
	{
		codegenexp(curr->child);
		codegenexp(curr->child->sibling);
		printf("\tpop r9\n");
		printf("\tpop r8\n");
		int firstfalselabel = getlabel();
		int secondtruelabel = getlabel();
		int endlabel = getlabel();
		printf("\tcmp r8, 0000000000000001h\n");
		printf("\tjne label_%d\n", firstfalselabel);
		printf("\tmov r8, 0000000000000001h\n");
		printf("\tpush r8\n");
		printf("\tjmp label_%d\n", endlabel);
		printf("label_%d:\n", firstfalselabel);
		printf("\tcmp r9, 0000000000000001h\n");
		printf("\tje label_%d\n", secondtruelabel);
		printf("\tmov r8, 0000000000000000h\n");
		printf("\tpush r8\n");
		printf("\tjmp label_%d\n", endlabel);
		printf("label_%d:\n",secondtruelabel);
		printf("\tmov r8, 0000000000000001h\n");
		printf("\tpush r8\n");
		printf("label_%d:\n",endlabel);
	}
	return;
}

int getlabel()
{
	static int x = 0;
	return x++;
}

void codegeniterative(stacknode* temp)
{
	if(strcmp(temp->child->ntortinfo->str, "FOR") == 0)
	{
		printf("\tmov r8, %s\n", temp->child->sibling->sibling->child->tokinfo->lexeme);
		printf("\tpush r8\n");
		int startlabel = getlabel();
		int endlabel = getlabel();
		printf("label_%d:\n", startlabel);
		printf("\tpop r8\n");
		printf("\tpush r8\n");
		printf("\tmov [%s], r8w\n", temp->child->sibling->tokinfo->lexeme);
		printf("\tmov r9, %s\n", temp->child->sibling->sibling->child->sibling->tokinfo->lexeme);
		printf("\tcmp r8, r9\n");
		printf("\tjg label_%d\n", endlabel);

		stacknode* temp2 = temp->child->sibling->sibling->sibling->sibling->child;
		while(temp2 != NULL)
		{
			code_statement(temp2);
			temp2 = temp2->sibling;
		}

		printf("\tpop r8\n");
		printf("\tinc r8\n");
		printf("\tpush r8\n");
		printf("\tjmp label_%d\n", startlabel);
		printf("label_%d: \n", endlabel);
	}
	if(strcmp(temp->child->ntortinfo->str, "WHILE") == 0)
	{
		int startlabel = getlabel();
		int endlabel = getlabel();

		printf("label_%d:\n", startlabel);	
		codegenexp(temp->child->sibling);
		printf("\tpop r8\n");
		printf("\tcmp r8, 0000000000000001h\n");
		printf("\tjne label_%d\n", endlabel);
		stacknode* temp2 = temp->child->sibling->sibling->sibling->child;
		while(temp2 != NULL)
		{
			code_statement(temp2);
			temp2 = temp2->sibling;
		}
		printf("\tjmp label_%d\n", startlabel);
		printf("label_%d:\n", endlabel);
	}
	return;
}

void codegenconditional(stacknode* curr)
{	
	idsymboltable* idst = curr->child->idst;
	idsymboltablenode* pt = getidsymboltablenode(curr->child->tokinfo->lexeme, idst);

	printf("\tmov r8, [%s]\n", pt->idlex);

	if(gettype(pt->type) == integer)
	{
		printf("\tand r8, 00000000000000ffh\n");
		printf("\tpush r8\n");

		stacknode* temp = curr->child->sibling->sibling; //<caseStmts>
		stacknode* temp2 = temp->child;

		int currlabel = getlabel();
		int endlabel = getlabel();
		while(temp2 != NULL)
		{
			int nextlabel = getlabel();
			printf("label_%d:\n", currlabel);
			printf("\tpop r8\n");
			printf("\tpush r8\n");
			printf("\tmov r9, %s\n", temp2->child->tokinfo->lexeme);
			printf("\tcmp r8,r9\n");
			printf("\tjne label_%d\n", nextlabel);
			stacknode* temp3 = temp2->sibling->child;
			while(temp3 != NULL)
			{
				code_statement(temp3);
				temp3 = temp3->sibling;
			}
			printf("\tjmp label_%d\n", endlabel);
			currlabel = nextlabel;
			temp2 = temp2->sibling->sibling;
		}
		temp2 = temp->sibling->child->child;
		printf("label_%d:\n", currlabel);
		while(temp2 != NULL)
		{
			code_statement(temp2);
			temp2 = temp2->sibling;
		}

		printf("label_%d:\n", endlabel);
	}
	else
	{
		printf("\tand r8, 0000000000000001h\n");
		printf("\tpush r8\n");

		stacknode* temp = curr->child->sibling->sibling;
		stacknode* temp2 = temp->child;

		int currlabel = getlabel();
		int endlabel = getlabel();
		int nextlabel;
		while(temp2 != NULL)
		{
			nextlabel = getlabel();
			printf("label_%d:\n", currlabel);
			printf("\tpop r8\n"); //pop id from stack
			printf("\tpush r8\n"); //push id to stack
			if(strcmp(temp2->child->tokinfo->lexeme, "true") == 0)
				printf("\tmov r9, 0000000000000001h\n");
			else
				printf("\tmov r9, 0000000000000000h\n");
			printf("\tcmp r8,r9\n");
			printf("\tjne label_%d\n", nextlabel);
			stacknode* temp3 = temp2->sibling->child;
			while(temp3 != NULL)
			{
				code_statement(temp3);
				temp3 = temp3->sibling;
			}
			printf("\tjmp label_%d\n",endlabel);
			currlabel = nextlabel;
			temp2 = temp2->sibling->sibling;
		}
		printf("label_%d:\n", nextlabel);
		printf("label_%d:\n", endlabel);
	}
	return;
}

void code_statement(stacknode* temp)
{
	if(strcmp(temp->ntortinfo->str, "<declareStmt>") == 0)
		return;
	if(strcmp(temp->ntortinfo->str, "<ioStmt>") == 0)
	{
	// 	mov rdi, scanint
	// lea rsi, [x]
	// mov al,0
	// call scanf
		if(strcmp(temp->child->ntortinfo->str, "GET_VALUE") == 0)
		{
			// idsymboltable* idst = temp->child->sibling->idst;
			// idsymboltablenode* pt = getidsymboltablenode(temp->child->sibling->tokinfo->lexeme, idst);

			printf("\tmov rdi, get_val\n");
			printf("\tlea rsi, [%s]\n", temp->child->sibling->tokinfo->lexeme);
			printf("\tmov al, 0\n");
			printf("\tcall scanf\n");
			printf("\n");
			return;
		}
		else if(strcmp(temp->child->ntortinfo->str, "PRINT") == 0)
		{
			stacknode* temp2 = temp->child->sibling->child;
			if(strcmp(temp2->ntortinfo->str, "NUM") == 0)
			{
				printf("\tmov rdi, print_val\n");
				printf("\tmov rsi, %s\n", temp2->tokinfo->lexeme);
				printf("\tmov al, 0\n");
				printf("\tcall printf\n");
				printf("\n");
			}
			else
			{
				idsymboltable* idst = temp2->idst;
				idsymboltablenode* pt = getidsymboltablenode(temp2->tokinfo->lexeme, idst);
				printf("\tmov rdi, print_val\n");
				printf("\tmov rax, [%s]\n", temp2->tokinfo->lexeme);
				if(pt->widthofid == 2)
					printf("\tand rax, 00000000000000ffh\n");
				else
					printf("\tand rax, 000000000000000fh\n");
				printf("\tmov rsi, rax\n");
				printf("\tmov al, 0\n");
				printf("\tcall printf\n");
				printf("\n");
			}
		}
	}

	if(strcmp(temp->ntortinfo->str, "<assignmentStmt>") == 0)
	{
		if(strcmp(temp->child->sibling->ntortinfo->str, "<lvalueIDStmt>") == 0)
		{
			codegenexp(temp->child->sibling->child);
			printf("\tpop r8\n");
			idsymboltable* idst = temp->child->idst;
			idsymboltablenode* pt = getidsymboltablenode(temp->child->tokinfo->lexeme, idst);
			if(pt->widthofid == 2)
				printf("\tmov [%s], r8w\n", temp->child->tokinfo->lexeme);//for integers
			else
				printf("\tmov [%s], r8b\n", temp->child->tokinfo->lexeme);//for booleans
		}
	}

	if(strcmp(temp->ntortinfo->str, "<iterativeStmt>") == 0)
	{
		codegeniterative(temp);
	}

	if(strcmp(temp->ntortinfo->str, "<condionalStmt>") == 0)
	{
		codegenconditional(temp);
	}
	return;
}

void traverseAST_forCodegen(stacknode* curr)
{
	if(curr == NULL)
		return;

	stacknode* temp = curr->child->sibling->sibling;//<driverModule>
	temp = temp->child->sibling;//<moduleDef>
	temp = temp->child->sibling->child;//<statements>->child

	while(temp != NULL)
	{
		code_statement(temp);
		temp = temp->sibling;
	}
	// traverseAST_forCodegen(curr->child);

	// if(strcmp(curr->ntortinfo->str, "<statements>") == 0)
	// {
	// 	stacknode* temp = curr->child;
	// 	while(temp != NULL)
	// 	{
	// 		code_statement(temp);
	// 		temp = temp->sibling;
	// 	}
	// }

	// if(curr->child == NULL)
	// 	return;
	
	// stacknode* temp = curr->child;
	// while(temp != NULL)
	// {
	// 	traverseAST_forCodegen(temp);
	// 	temp = temp->sibling;
	// }
	return;
}

void generate_code(mainsymboltable* globalTable, stacknode* astroot)
{
	initialize(globalTable);
	traverseAST_forCodegen(astroot);
	endcode();
}