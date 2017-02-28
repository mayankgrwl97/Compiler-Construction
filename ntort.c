#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ntort.h"

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

void printntortnode(ntort* node)
{
	printf("| %d %d %s | ",node->nt,node->val,node->str);
	return;
}