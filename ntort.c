/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ntort.h"


//Non Terminal or Terminal node
ntort* makentortnode(int nt, int val, char* str)
{
	ntort* temp = (ntort*)malloc(sizeof(ntort));
	temp->nt = nt;
	temp->val = val;
	temp->next = temp->down = NULL;
	int l = strlen(str);
	temp->str = (char*)malloc((1+l)*sizeof(char));
	strcpy(temp->str, str);
	return temp;
}

void printntortnode(ntort* node)
{
	printf("| %d %d %s | ",node->nt,node->val,node->str);
	return;
}