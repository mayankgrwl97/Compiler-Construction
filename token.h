/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

#ifndef _token_h_
#define _token_h_

typedef struct tokeninfo tokeninfo;

struct tokeninfo
{
	char* tokenname;
	int linenumber;
	char* lexeme;
	tokeninfo* next;
};

tokeninfo* makeToken(char* tokenname, char* lexeme, int linenumber);

#endif