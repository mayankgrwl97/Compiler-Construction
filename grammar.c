#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define hashtablesize 100

typedef struct ntort ntort;
struct ntort
{
	int nt;
	int val;
	char* str;
	ntort* next;
};

typedef struct hashtable hashtable;
struct hashtable
{
	ntort* buckets[hashtablesize];
};

ntort* makentortnode(int nt, int val, char* str)
{
	ntort* temp = (ntort*)malloc(sizeof(ntort));
	temp->nt = nt;
	temp->val = val;
	int l = strlen(str);
	// str[l-1] = '\0';
	temp->str = (char*)malloc((1+l)*sizeof(char));
	strcpy(temp->str, str);
	return temp;
}

int hash(char* str)
{
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
	    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return (int)(hash % hashtablesize);
}

void printntortnode(ntort* node)
{
	printf("| %d %d %s | ",node->nt,node->val,node->str);
	return;
}

void printhashtable(hashtable* table)
{
	for(int i=0; i<hashtablesize; i++)
	{
		printf("%d -> ",i);
		ntort* pt = table->buckets[i];
		while(pt != NULL)
		{
			printntortnode(pt);
			pt=pt->next;
		}
		printf("\n");
	}

	return;
}

hashtable* makehashtable()
{
	hashtable* temp = (hashtable*)malloc(sizeof(hashtable));
	for(int i=0; i<hashtablesize; i++)
		temp->buckets[i] = NULL;
	return temp;
}

int present(hashtable* table, char* str)
{
	int h = hash(str);
	ntort* pt = table->buckets[h];

	while(pt != NULL)
	{
		if(strcmp(str,pt->str) == 0)
			return pt->val;
		pt = pt->next;
	}
	return 0;
}

void insert(hashtable* table, char* str)
{
	// printf("%s\n",str);
	static int ntval=1, tval=1;
	int h = hash(str);
	ntort* pt = table->buckets[h];
	int nt;
	if(str[0] == '<')
	{
		nt = 1;
		ntort* newnode = makentortnode(nt, ntval,str);
		ntval++;
		newnode->next = pt;
		table->buckets[h] = newnode;
	}
	else
	{
		nt = 0;
		ntort* newnode = makentortnode(nt, tval,str);
		tval++;
		newnode->next = pt;
		table->buckets[h] = newnode;
	}
	return;
}

typedef struct node node;
struct node
{
	node* next;
	ntort* firstntort;
	ntort* lastntort;
};

node* makenode()
{
	node* temp = (node*)malloc(sizeof(node));
	temp->next = NULL;
	temp->firstntort = NULL;
	temp->lastntort = NULL;
	return temp;
}

node* grammar[60];
node* toppointers[60];

char buff[500];


/*ntort* makentortnode(int nt, int val, char* str);
int hash(char* str);
hashtable* makehashtable();
int present(hashtable* table, char* str);
void insert(hashtable* table, char* str);*/

void populateGrammar(hashtable* table)
{
	FILE* fp = fopen("newGrammar.txt","rb");
	for(int i=0; i<51; i++)
	{
		fscanf(fp,"%s",buff); //scanning LHS nonterminal
		if(!present(table, buff))
			insert(table, buff);
		
		int ind = present(table, buff);

		fscanf(fp, "%s", buff); //scanning -->
		fscanf(fp, "%s", buff);
		// printf("%s\n", buff);

		grammar[ind] = makenode();
		node* curr = grammar[ind];

		while(buff[0] != '$')
		{
			if(buff[0] == '|')
			{
				curr->next = makenode();
				curr = curr->next;
			}
			else
			{
				if(!present(table, buff))
					insert(table, buff);
				int rhsind = present(table, buff);

				if(curr->firstntort == NULL)
				{
					curr->firstntort = makentortnode(buff[0]=='<' ? 1 : 0, rhsind, buff);
					curr->lastntort = curr->firstntort;
				}
				else
				{
					curr->lastntort->next = makentortnode(buff[0]=='<' ? 1 : 0, rhsind, buff);
					curr->lastntort = curr->lastntort->next;
				}
			}
			fscanf(fp, "%s", buff);
		}
	}
	// printhashtable(table);
	fclose(fp);
	return;
}

void printGrammar(hashtable* table)
{
	FILE* fp = fopen("nonterminals.txt", "rb");
	for(int i=0; i<51; i++)
	{
		fscanf(fp,"%s",buff);
		int ind = present(table, buff);

		printf("%s --> ", buff);

		if(ind == 0)
		{
			printf("########ERROR######### %s\n", buff);
			return;
		}
		node* curr = grammar[ind];
		while(curr != NULL)
		{
			ntort* first = curr->firstntort;
			while(first != NULL)
			{
				printf(" %s ",first->str);
				first = first->next;
			}
			printf("|");
			curr = curr->next;
		}
		printf("\n");
	}
	return ;
}

int main()
{
	hashtable* table = makehashtable();
	populateGrammar(table);
	printGrammar(table);
	return 0;
}