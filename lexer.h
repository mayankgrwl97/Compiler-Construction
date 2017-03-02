#ifndef _lexer_h_
#define _lexer_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "token.h"
#include "symboltable.h"

char* upperstr(char* str);
char* getStream(FILE** fp);
int isdelim(char ch);
void addKeywords(symboltable* table);
void removeComments(FILE* fp);
tokeninfo* getNextToken(FILE* cleanFile, symboltable* table);
tokeninfo* getAllTokens(FILE* testCaseFile);

#endif