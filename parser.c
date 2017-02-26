#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
	str[l-1] = '\0';
	temp->str = (char*)malloc(l*sizeof(char));
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
			return 1;
		pt = pt->next;
	}
	return 0;
}

void insert(hashtable* table, char* str)
{
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

int main()
{
	hashtable* table = makehashtable();
	FILE* fp = fopen("nonterminals.txt", "r");
	// printhashtable(table);
	char buff[100];
	while(fgets(buff, 100, fp))
	{
		// printf("%s",buff);
		insert(table, buff);
		// fgets(buff,100,fp);
	}
	// insert(table, "<program>");
	// insert(table, "eps");
	// insert(table, "COMMA");
	printhashtable(table);
	// printf("%d\n", present(table, "eps"));

	return 0;
}