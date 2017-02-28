#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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