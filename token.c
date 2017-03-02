/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

#include <stdlib.h>
#include <string.h>
#include "token.h"

tokeninfo* makeToken(char* tokenname, char* lexeme, int linenumber)
{
	tokeninfo* temp = (tokeninfo*)malloc(sizeof(tokeninfo));
	int l = strlen(tokenname);
	temp->tokenname = (char*)malloc((l+1)* (sizeof(char)));
	strcpy(temp->tokenname, tokenname);

	l = strlen(lexeme);
	temp->lexeme = (char*)malloc((l+1)* (sizeof(char)));
	strcpy(temp->lexeme, lexeme);	

	temp->linenumber = linenumber;
	temp->next = NULL;
	return temp;
}
