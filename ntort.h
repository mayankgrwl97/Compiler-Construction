#ifndef _ntort_h_
#define _ntort_h_

typedef struct ntort ntort;
struct ntort
{
	int nt;
	int val;
	char* str;
	ntort* next;
};

ntort* makentortnode(int nt, int val, char* str);
void printntortnode(ntort* node);

#endif