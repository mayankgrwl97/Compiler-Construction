/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

#ifndef _lexer_h_
#define _lexer_h_

#include "lexerDef.h"

char* upperstr(char* str);
char* getStream(FILE** fp);
int isdelim(char ch);
void addKeywords(symboltable* table);
void removeComments(FILE* fp);
tokeninfo* getNextToken(FILE* cleanFile, symboltable* table);
tokeninfo* getAllTokens(FILE* testCaseFile);

#endif