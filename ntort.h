/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

#ifndef _ntort_h_
#define _ntort_h_

typedef struct ntort ntort;
struct ntort
{
	int nt; //nt = 1 if str is non terminal
	int val; //value assigned by hashtable (mapping from string to int)
	char* str;
	ntort* next;
	ntort* down; //used in grammar linking
};

ntort* makentortnode(int nt, int val, char* str);
void printntortnode(ntort* node);

#endif