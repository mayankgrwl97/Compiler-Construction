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

void endcode(FILE* fp)
{
	fprintf(fp, "\tmov eax, 1\n");
	fprintf(fp, "\tmov ebx, 1\n");
	fprintf(fp, "\tint 80h\n");
	// fprintf(fp, "\tret\n");
	return;
}

void initialize(FILE* fp, mainsymboltable* globalTable)
{
	fprintf(fp, "SECTION .bss\n");
	// ffprintf(fp, fp, "")
	mainsymboltablenode* temp = presentmainsymboltable(globalTable, "program");
	idsymboltable* pt = temp->idst;
	for(int i=0; i<idsymboltablesize; i++)
	{
		if(pt->buckets[i] != NULL)
		{
			idsymboltablenode* curr = pt->buckets[i];
			while(curr != NULL)
			{
				fprintf(fp, "%s\tresb %d\n", curr->idlex, 2*curr->widthofid);
				curr = curr->next;
			}
		}
	}

	fprintf(fp, "\n");

	fprintf(fp, "SECTION .data\n");
	fprintf(fp, "get_val: db \"%%d\", 0\n");
	fprintf(fp, "print_val: db \"%%d\", 10, 0\n\n");
	fprintf(fp, "SECTION .text\n\n");
	fprintf(fp, "extern printf\n");
	fprintf(fp, "extern scanf\n\n");
	fprintf(fp, "GLOBAL main\n\n");

	fprintf(fp, "main:\n");
	return;
}

void codegenexp(FILE* fp, stacknode* curr)
{
	if(strcmp(curr->ntortinfo->str, "<expression>") == 0)
		codegenexp(fp, curr->child);

	if(strcmp(curr->ntortinfo->str, "<var>") == 0)
	{
		if(strcmp(curr->child->ntortinfo->str, "NUM") == 0)
		{
			fprintf(fp, "\tmov r8, %s\n", curr->child->tokinfo->lexeme);
			fprintf(fp, "\tpush r8\n");
			return;
		}
		else if(strcmp(curr->child->ntortinfo->str, "RNUM") == 0)
		{
			// not to be considered
		}
		else
		{			
			idsymboltablenode* temp = getidsymboltablenode(curr->child->tokinfo->lexeme, curr->child->idst);
			fprintf(fp, "\tmov r8, [%s]\n", temp->idlex);
			fprintf(fp, "\tand r8, 00000000000000ffh\n");
			fprintf(fp, "\tpush r8\n");
		}
	}

	if(strcmp(curr->ntortinfo->str, "MINUS") == 0)
	{
		if(curr->sibling != NULL)
		{
			codegenexp(fp, curr->sibling);
			fprintf(fp, "\tpop r9\n");
			fprintf(fp, "\tmov r8, 0\n");
			fprintf(fp, "\tsub r8, r9\n");
			fprintf(fp, "\tpush r8\n");
		}
		else
		{
			codegenexp(fp, curr->child);
			codegenexp(fp, curr->child->sibling);
			fprintf(fp, "\tpop r9\n");
			fprintf(fp, "\tpop r8\n");
			fprintf(fp, "\tsub r8,r9\n");
			fprintf(fp, "\tpush r8\n");
		}
	}

	if(strcmp(curr->ntortinfo->str, "PLUS") == 0)
	{
		codegenexp(fp, curr->child);
		codegenexp(fp, curr->child->sibling);
		fprintf(fp, "\tpop r9\n");
		fprintf(fp, "\tpop r8\n");
		fprintf(fp, "\tadd r8,r9\n");
		fprintf(fp, "\tpush r8\n");
	}

	if(strcmp(curr->ntortinfo->str, "MUL") == 0)
	{
		codegenexp(fp, curr->child);
		codegenexp(fp, curr->child->sibling);
		fprintf(fp, "\tpop r9\n");
		fprintf(fp, "\tpop r8\n");
		fprintf(fp, "\timul r8,r9\n");
		fprintf(fp, "\tpush r8\n");
	}

	if(strcmp(curr->ntortinfo->str, "DIV") == 0)
	{
		codegenexp(fp, curr->child);
		codegenexp(fp, curr->child->sibling);
		fprintf(fp, "\tpop r9\n");
		fprintf(fp, "\tpop r8\n");
		fprintf(fp, "\tdiv r8,r9\n");
		fprintf(fp, "\tpush r8\n");
	}

	if((strcmp(curr->ntortinfo->str, "LT") == 0) || (strcmp(curr->ntortinfo->str, "LE") == 0) || (strcmp(curr->ntortinfo->str, "GT") == 0) || (strcmp(curr->ntortinfo->str, "GE") == 0) || (strcmp(curr->ntortinfo->str, "EQ") == 0) || (strcmp(curr->ntortinfo->str, "NE") == 0))
	{
		codegenexp(fp, curr->child);
		codegenexp(fp, curr->child->sibling);
		int truelabel = getlabel();
		int endlabel = getlabel();
		fprintf(fp, "\tpop r9\n");
		fprintf(fp, "\tpop r8\n");
		fprintf(fp, "\tcmp r8,r9\n");
		if(strcmp(curr->ntortinfo->str, "LT") == 0)
			fprintf(fp, "\tjl truelabel_%d\n", truelabel);
		else if(strcmp(curr->ntortinfo->str, "LE") == 0)
			fprintf(fp, "\tjle truelabel_%d\n", truelabel);
		else if(strcmp(curr->ntortinfo->str, "GT") == 0)
			fprintf(fp, "\tjg truelabel_%d\n", truelabel);
		else if(strcmp(curr->ntortinfo->str, "GE") == 0)
			fprintf(fp, "\tjge truelabel_%d\n", truelabel);
		else if(strcmp(curr->ntortinfo->str, "EQ") == 0)
			fprintf(fp, "\tje truelabel_%d\n", truelabel);
		else if(strcmp(curr->ntortinfo->str, "NEQ") == 0)
			fprintf(fp, "\tjne truelabel_%d\n", truelabel);
		fprintf(fp, "\tmov r8, 0\n");
		fprintf(fp, "\tpush r8\n");
		fprintf(fp, "\tjmp label_%d\n", endlabel);
		fprintf(fp, "truelabel_%d:\n", truelabel);
		fprintf(fp, "\tmov r8, 1\n");
		fprintf(fp, "\tpush r8\n");
		fprintf(fp, "label_%d:\n", endlabel);
	}

	if(strcmp(curr->ntortinfo->str, "AND") == 0)
	{
		codegenexp(fp, curr->child);
		codegenexp(fp, curr->child->sibling);
		fprintf(fp, "\tpop r9\n");
		fprintf(fp, "\tpop r8\n");
		int firsttruelabel = getlabel();
		int secondfalselabel = getlabel();
		int endlabel = getlabel();
		fprintf(fp, "\tcmp r8, 0000000000000001h\n");
		fprintf(fp, "\tje label_%d\n", firsttruelabel);
		fprintf(fp, "\tmov r8, 0000000000000000h\n");
		fprintf(fp, "\tpush r8\n");
		fprintf(fp, "\tjmp label_%d\n", endlabel);
		fprintf(fp, "label_%d:\n", firsttruelabel);
		fprintf(fp, "\tcmp r9, 0000000000000001h\n");
		fprintf(fp, "\tjne label_%d\n", secondfalselabel);
		fprintf(fp, "\tmov r8, 0000000000000001h\n");
		fprintf(fp, "\tpush r8\n");
		fprintf(fp, "\tjmp label_%d\n", endlabel);
		fprintf(fp, "label_%d:\n",secondfalselabel);
		fprintf(fp, "\tmov r8, 0000000000000000h\n");
		fprintf(fp, "\tpush r8\n");
		fprintf(fp, "label_%d:\n",endlabel);
	}

	if(strcmp(curr->ntortinfo->str, "OR") == 0)
	{
		codegenexp(fp, curr->child);
		codegenexp(fp, curr->child->sibling);
		fprintf(fp, "\tpop r9\n");
		fprintf(fp, "\tpop r8\n");
		int firstfalselabel = getlabel();
		int secondtruelabel = getlabel();
		int endlabel = getlabel();
		fprintf(fp, "\tcmp r8, 0000000000000001h\n");
		fprintf(fp, "\tjne label_%d\n", firstfalselabel);
		fprintf(fp, "\tmov r8, 0000000000000001h\n");
		fprintf(fp, "\tpush r8\n");
		fprintf(fp, "\tjmp label_%d\n", endlabel);
		fprintf(fp, "label_%d:\n", firstfalselabel);
		fprintf(fp, "\tcmp r9, 0000000000000001h\n");
		fprintf(fp, "\tje label_%d\n", secondtruelabel);
		fprintf(fp, "\tmov r8, 0000000000000000h\n");
		fprintf(fp, "\tpush r8\n");
		fprintf(fp, "\tjmp label_%d\n", endlabel);
		fprintf(fp, "label_%d:\n",secondtruelabel);
		fprintf(fp, "\tmov r8, 0000000000000001h\n");
		fprintf(fp, "\tpush r8\n");
		fprintf(fp, "label_%d:\n",endlabel);
	}
	return;
}

int getlabel()
{
	static int x = 0;
	return x++;
}

void codegeniterative(FILE* fp, stacknode* temp)
{
	if(strcmp(temp->child->ntortinfo->str, "FOR") == 0)
	{
		fprintf(fp, "\tmov r8, %s\n", temp->child->sibling->sibling->child->tokinfo->lexeme);
		fprintf(fp, "\tpush r8\n");
		int startlabel = getlabel();
		int endlabel = getlabel();
		fprintf(fp, "label_%d:\n", startlabel);
		fprintf(fp, "\tpop r8\n");
		fprintf(fp, "\tpush r8\n");
		fprintf(fp, "\tmov [%s], r8w\n", temp->child->sibling->tokinfo->lexeme);
		fprintf(fp, "\tmov r9, %s\n", temp->child->sibling->sibling->child->sibling->tokinfo->lexeme);
		fprintf(fp, "\tcmp r8, r9\n");
		fprintf(fp, "\tjg label_%d\n", endlabel);

		stacknode* temp2 = temp->child->sibling->sibling->sibling->sibling->child;
		while(temp2 != NULL)
		{
			code_statement(fp, temp2);
			temp2 = temp2->sibling;
		}

		fprintf(fp, "\tpop r8\n");
		fprintf(fp, "\tinc r8\n");
		fprintf(fp, "\tpush r8\n");
		fprintf(fp, "\tjmp label_%d\n", startlabel);
		fprintf(fp, "label_%d: \n", endlabel);
		fprintf(fp, "\tpop r8\n");
	}
	if(strcmp(temp->child->ntortinfo->str, "WHILE") == 0)
	{
		int startlabel = getlabel();
		int endlabel = getlabel();

		fprintf(fp, "label_%d:\n", startlabel);	
		codegenexp(fp, temp->child->sibling);
		fprintf(fp, "\tpop r8\n");
		fprintf(fp, "\tcmp r8, 0000000000000001h\n");
		fprintf(fp, "\tjne label_%d\n", endlabel);
		stacknode* temp2 = temp->child->sibling->sibling->sibling->child;
		while(temp2 != NULL)
		{
			code_statement(fp, temp2);
			temp2 = temp2->sibling;
		}
		fprintf(fp, "\tjmp label_%d\n", startlabel);
		fprintf(fp, "label_%d:\n", endlabel);
	}
	return;
}

void codegenconditional(FILE* fp, stacknode* curr)
{	
	idsymboltable* idst = curr->child->idst;
	idsymboltablenode* pt = getidsymboltablenode(curr->child->tokinfo->lexeme, idst);

	fprintf(fp, "\tmov r8, [%s]\n", pt->idlex);

	if(gettype(pt->type) == integer)
	{
		fprintf(fp, "\tand r8, 00000000000000ffh\n");
		fprintf(fp, "\tpush r8\n");

		stacknode* temp = curr->child->sibling->sibling; //<caseStmts>
		stacknode* temp2 = temp->child;

		int currlabel = getlabel();
		int endlabel = getlabel();
		while(temp2 != NULL)
		{
			int nextlabel = getlabel();
			fprintf(fp, "label_%d:\n", currlabel);
			fprintf(fp, "\tpop r8\n");
			fprintf(fp, "\tpush r8\n");
			fprintf(fp, "\tmov r9, %s\n", temp2->child->tokinfo->lexeme);
			fprintf(fp, "\tcmp r8,r9\n");
			fprintf(fp, "\tjne label_%d\n", nextlabel);
			stacknode* temp3 = temp2->sibling->child;
			while(temp3 != NULL)
			{
				code_statement(fp, temp3);
				temp3 = temp3->sibling;
			}
			fprintf(fp, "\tjmp label_%d\n", endlabel);
			currlabel = nextlabel;
			temp2 = temp2->sibling->sibling;
		}
		temp2 = temp->sibling->child->child;
		fprintf(fp, "label_%d:\n", currlabel);
		while(temp2 != NULL)
		{
			code_statement(fp, temp2);
			temp2 = temp2->sibling;
		}

		fprintf(fp, "label_%d:\n", endlabel);
	}
	else
	{
		fprintf(fp, "\tand r8, 0000000000000001h\n");
		fprintf(fp, "\tpush r8\n");

		stacknode* temp = curr->child->sibling->sibling;
		stacknode* temp2 = temp->child;

		int currlabel = getlabel();
		int endlabel = getlabel();
		int nextlabel;
		while(temp2 != NULL)
		{
			nextlabel = getlabel();
			fprintf(fp, "label_%d:\n", currlabel);
			fprintf(fp, "\tpop r8\n"); //pop id from stack
			fprintf(fp, "\tpush r8\n"); //push id to stack
			if(strcmp(temp2->child->tokinfo->lexeme, "true") == 0)
				fprintf(fp, "\tmov r9, 0000000000000001h\n");
			else
				fprintf(fp, "\tmov r9, 0000000000000000h\n");
			fprintf(fp, "\tcmp r8,r9\n");
			fprintf(fp, "\tjne label_%d\n", nextlabel);
			stacknode* temp3 = temp2->sibling->child;
			while(temp3 != NULL)
			{
				code_statement(fp, temp3);
				temp3 = temp3->sibling;
			}
			fprintf(fp, "\tjmp label_%d\n",endlabel);
			currlabel = nextlabel;
			temp2 = temp2->sibling->sibling;
		}
		fprintf(fp, "label_%d:\n", nextlabel);
		fprintf(fp, "label_%d:\n", endlabel);
	}
	return;
}

void code_statement(FILE* fp, stacknode* temp)
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
			idsymboltable* idst = temp->child->sibling->idst;
			idsymboltablenode* pt = getidsymboltablenode(temp->child->sibling->tokinfo->lexeme, idst);

			if(strcmp(pt->type->ntortinfo->str, "INTEGER") == 0)
			{
				fprintf(fp, "\tmov rdi, get_val\n");
				fprintf(fp, "\tlea rsi, [%s]\n", temp->child->sibling->tokinfo->lexeme);
				fprintf(fp, "\tmov al, 0\n");
				fprintf(fp, "\tcall scanf\n");
				fprintf(fp, "\n");
			}

			else if(strcmp(pt->type->ntortinfo->str, "ARRAY") == 0)
			{
				if(strcmp(pt->type->child->sibling->ntortinfo->str, "INTEGER") == 0)
				{
					int startlabel = getlabel();
					int endlabel = getlabel();
					fprintf(fp, "\tmov r11, 0\n"); //array curr index 
					fprintf(fp, "\tmov r8, %s\n", pt->type->child->child->tokinfo->lexeme);
					fprintf(fp, "\tpush r8\n");
					fprintf(fp, "label_%d:\n",startlabel);
					fprintf(fp, "\tpop r8\n");
					fprintf(fp, "\tpush r8\n");
					fprintf(fp, "\tmov r9, %s\n", pt->type->child->child->sibling->tokinfo->lexeme);
					fprintf(fp, "\tcmp r8, r9\n");
					fprintf(fp, "\tjg label_%d\n",endlabel);
					fprintf(fp, "\tmov rdi, get_val\n");
					fprintf(fp, "\tlea rsi, [%s]\n", pt->idlex);
					fprintf(fp, "\tadd rsi, r11\n");
					fprintf(fp, "\tmov al,0\n");
					fprintf(fp, "\tpush r11\n");
					fprintf(fp, "\tcall scanf\n");
					fprintf(fp, "\tpop r11\n");
					fprintf(fp, "\tadd r11, 4\n");
					fprintf(fp, "\tpop r8\n");
					fprintf(fp, "\tinc r8\n");
					fprintf(fp, "\tpush r8\n");
					fprintf(fp, "\tjmp label_%d\n", startlabel);
					fprintf(fp, "label_%d:\n", endlabel);
					fprintf(fp, "\tpop r8\n");
				}
			}
			return;
		}
		else if(strcmp(temp->child->ntortinfo->str, "PRINT") == 0)
		{
			stacknode* temp2 = temp->child->sibling->child;
			if(strcmp(temp2->ntortinfo->str, "NUM") == 0)
			{
				fprintf(fp, "\tmov rdi, print_val\n");
				fprintf(fp, "\tmov rsi, %s\n", temp2->tokinfo->lexeme);
				fprintf(fp, "\tmov al, 0\n");
				fprintf(fp, "\tcall printf\n");
				fprintf(fp, "\n");
			}
			else
			{
				idsymboltable* idst = temp2->idst;
				idsymboltablenode* pt = getidsymboltablenode(temp2->tokinfo->lexeme, idst);
				if(pt->widthofid == 1 || pt->widthofid == 2)
				{
					fprintf(fp, "\tmov rdi, print_val\n");
					fprintf(fp, "\tmov rax, [%s]\n", temp2->tokinfo->lexeme);
					if(pt->widthofid == 2)
						fprintf(fp, "\tand rax, 00000000000000ffh\n");
					else if(pt->widthofid == 1)
						fprintf(fp, "\tand rax, 000000000000000fh\n");
					fprintf(fp, "\tmov rsi, rax\n");
					fprintf(fp, "\tmov al, 0\n");
					fprintf(fp, "\tcall printf\n");
					fprintf(fp, "\n");
				}
				else
				{
					if(strcmp(pt->type->child->sibling->ntortinfo->str, "INTEGER") == 0)
					{
						int startlabel = getlabel();
						int endlabel = getlabel();
						fprintf(fp, "\tmov r11, 0\n"); //array curr index 
						fprintf(fp, "\tmov r8, %s\n", pt->type->child->child->tokinfo->lexeme);
						fprintf(fp, "\tpush r8\n");
						fprintf(fp, "label_%d:\n",startlabel);
						fprintf(fp, "\tpop r8\n");
						fprintf(fp, "\tpush r8\n");
						fprintf(fp, "\tmov r9, %s\n", pt->type->child->child->sibling->tokinfo->lexeme);
						fprintf(fp, "\tcmp r8, r9\n");
						fprintf(fp, "\tjg label_%d\n",endlabel);

						fprintf(fp, "\tmov rdi, print_val\n");
						fprintf(fp, "\tmov rax, [%s+r11]\n", pt->idlex);
						fprintf(fp, "\tand rax, 00000000000000ffh\n");
						// fprintf(fp, "\tadd rsi, r11\n");
						fprintf(fp, "\tmov rsi, rax\n");
						fprintf(fp, "\tmov al,0\n");
						fprintf(fp, "\tpush r11\n");
						fprintf(fp, "\tcall printf\n");

						fprintf(fp, "\tpop r11\n");
						fprintf(fp, "\tadd r11, 4\n");
						fprintf(fp, "\tpop r8\n");
						fprintf(fp, "\tinc r8\n");
						fprintf(fp, "\tpush r8\n");
						fprintf(fp, "\tjmp label_%d\n", startlabel);
						fprintf(fp, "label_%d:\n", endlabel);
						fprintf(fp, "\tpop r8\n");
					}
				}
			}
		}
	}

	if(strcmp(temp->ntortinfo->str, "<assignmentStmt>") == 0)
	{
		if(strcmp(temp->child->sibling->ntortinfo->str, "<lvalueIDStmt>") == 0)
		{
			codegenexp(fp, temp->child->sibling->child);
			fprintf(fp, "\tpop r8\n");
			idsymboltable* idst = temp->child->idst;
			idsymboltablenode* pt = getidsymboltablenode(temp->child->tokinfo->lexeme, idst);
			if(pt->widthofid == 2)
				fprintf(fp, "\tmov [%s], r8w\n", temp->child->tokinfo->lexeme);//for integers
			else
				fprintf(fp, "\tmov [%s], r8b\n", temp->child->tokinfo->lexeme);//for booleans
		}
	}

	if(strcmp(temp->ntortinfo->str, "<iterativeStmt>") == 0)
	{
		codegeniterative(fp, temp);
	}

	if(strcmp(temp->ntortinfo->str, "<condionalStmt>") == 0)
	{
		codegenconditional(fp, temp);
	}
	return;
}

void traverseAST_forCodegen(FILE* fp, stacknode* curr)
{
	if(curr == NULL)
		return;

	stacknode* temp = curr->child->sibling->sibling;//<driverModule>
	temp = temp->child->sibling;//<moduleDef>
	temp = temp->child->sibling->child;//<statements>->child

	while(temp != NULL)
	{
		code_statement(fp, temp);
		temp = temp->sibling;
	}
	return;
}

void generate_code(FILE* fp, mainsymboltable* globalTable, stacknode* astroot)
{
	initialize(fp, globalTable);
	traverseAST_forCodegen(fp, astroot);
	endcode(fp);
}