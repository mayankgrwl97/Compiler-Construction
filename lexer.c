/*
BATCH NO. 27
Mayank Agarwal (2014A7PS111P)
Karan Deep Batra(2014A7PS160P)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexerDef.h"
#include "token.h"
#include "symboltable.h"

char terminals[numberofterminals][20] = 
{
"NUM", "RNUM", "ID", "INTEGER", "REAL", "BOOLEAN", "OF", "ARRAY", "START", "END", "DECLARE", "MODULE", "DRIVER", "PROGRAM", "GET_VALUE", "PRINT", "USE", "WITH", "PARAMETERS", "TRUE", "FALSE", "TAKES",
"INPUT", "RETURNS", "AND", "OR", "FOR", "IN", "SWITCH", "CASE", "BREAK", "DEFAULT", "WHILE", "PLUS", "MINUS", "MUL", "DIV", "LT", "LE", "GE", "GT", "EQ", "NE", "DEF", "ENDDEF", "COLON", "RANGEOP", "SEMICOL",
"COMMA","ASSIGNOP","SQBO","SQBC","BO","BC","DRIVERDEF","DRIVERENDDEF","eps", "$"
};

char keywords[numberofkeywords][20] = 
{
"integer", "real", "boolean", "of", "array", "start", "end", "declare", "module", "driver", "program", "get_value", "print", "use", "with", "parameters", "true",
"false", "takes", "input", "returns", "AND", "OR", "for", "in", "switch", "case", "break", "default", "while"
};


char* upperstr(char* str) //returns string to uppercase
{
	int l = strlen(str);
	char* temp = malloc(sizeof(char) * (l+1));
	strcpy(temp, str);
	for(int i=0; i<l; i++)
		temp[i] = toupper(temp[i]);
	return temp;
}

char* getStream(FILE** fp) //takes input_size(100) bytes input in buffer
{
	char* testCaseFile;
	size_t inputsize = 100;
	testCaseFile = (char*)calloc(sizeof(char), inputsize * (sizeof(char)));
	fread(testCaseFile, sizeof(char), inputsize, *fp);
	return testCaseFile;
}

int isdelim(char ch) //returns true if ch is delimiter
{
	return (ch==' ' || ch=='\t' || ch=='\n' || ch=='\r' || ch==':' || ch=='=' 
		|| ch=='<' || ch=='>' || ch=='!' || ch=='(' || ch==')' 
		|| ch=='[' || ch==']' || ch==';' || ch=='+' || ch=='-' 
		|| ch=='*' || ch=='/' || ch==',');
}

void addKeywords(symboltable* table) //add all keywords in symboltable
{
	for(int i=0; i<numberofkeywords; i++)
		insertsymboltable(table, upperstr(keywords[i]), keywords[i], i+1);
	// printsymboltable(table);
	return;
}

void removeComments(FILE* fp) //fp is the filepointer for test case file //writes output in cleanFile.txt
{
	FILE* cleanFile = fopen("cleanFile.txt", "w+");
	char* testCaseFile = getStream(&fp);
	if(testCaseFile[0] == '\0')
		return ;

	int i=0;
	int flag = 0;

	while(1)
	{
		if(testCaseFile[i] == '\0')
		{
			testCaseFile = getStream(&fp);
			if(testCaseFile[0] == '\0')
				break;
			i = 0;
		}

		if(testCaseFile[i] == '*')//one star found
		{
			if(testCaseFile[i+1] == '\0')
			{
				testCaseFile = getStream(&fp);//check for errors
				if(testCaseFile[0] == '\0')
					return;
				i = -1;
			}

			if(testCaseFile[i+1] == '*')
			{
				i+=2;
				flag=1-flag;
			}
			else
			{
				if(flag == 0)
					fputc('*', cleanFile);
				i++;
			}
		}
		else
		{
			if(flag == 0)
				fputc(testCaseFile[i], cleanFile);
			i++;
		}
	}
	fclose(cleanFile);
	return ;
}

char* buff; //pointer to buffer from getStream
char val[100]; //to extract the lexeme
int i = 0; //index of character in buff
int linenumber = 1; //current linenumber

tokeninfo* getNextToken(FILE* cleanFile, symboltable* table) //returns next Token from clean File
{
	if(buff == NULL)// take buffer for the first time
		buff = getStream(&cleanFile);

	while(1)
	{
		if(buff == NULL || buff[i] == '\0')
		{
			buff = getStream(&cleanFile);
			i = 0;
			if(buff == NULL || buff[0] == '\0')
			{
				// printf("END OF CLEAN FILE");
				return makeToken("EOF", "EOF", linenumber);
			}
		}

		switch(buff[i])
		{
			case ' ':
			case '\t':
				i++;
				continue;
			case '\n':
				linenumber++;
				i++;
				continue;
			case '\r':
				i++;
				continue;
			case '+':
				i++;
				return makeToken("PLUS", "+", linenumber);
			case '-':
				i++;
				return makeToken("MINUS", "-", linenumber);
			case '*':
				i++;
				return makeToken("MUL", "*", linenumber);
			case '/':
				i++;
				return makeToken("DIV", "/", linenumber);
			case ';':
				i++;
				return makeToken("SEMICOL", ";", linenumber);
			case ',':
				i++;
				return makeToken("COMMA", ",", linenumber);
			case '(':
				i++;
				return makeToken("BO", "(", linenumber);
			case ')':
				i++;
				return makeToken("BC", ")", linenumber);
			case '[':
				i++;
				return makeToken("SQBO", "[", linenumber);
			case ']':
				i++;
				return makeToken("SQBC", "]", linenumber);
			case '<': //check for < | << | <<< | <=
				if(buff[i+1] == '\0')
				{
					buff = getStream(&cleanFile);
					if(buff[0] == '\0')
					{
						i = 0;
						return makeToken("LT", "<", linenumber);
					}
					i = -1;
				}
				switch(buff[i+1])
				{
					case '<':
						i++;
						if(buff[i+1] == '\0')
						{
							buff = getStream(&cleanFile);
							if(buff[0] == '\0')
							{
								i = 0;
								return makeToken("DEF", "<<", linenumber);
							}
							i = -1;
						}
						if(buff[i+1] == '<')
						{
							i += 2;
							return makeToken("DRIVERDEF", "<<<", linenumber);
						}
						i++;
						return makeToken("DEF", "<<", linenumber);
					case '=':
						i+=2;
						return makeToken("LE", "<=", linenumber);
					default:
						i++;
						return makeToken("LT", "<", linenumber);
				}
			case '>': //check for > | >> | >>> | >=
				if(buff[i+1] == '\0')
				{
					buff = getStream(&cleanFile);
					if(buff[0] == '\0')
					{
						i = 0;
						return makeToken("GT", ">", linenumber);
					}
					i = -1;
				}
				switch(buff[i+1])
				{
					case '>':
						i++;
						if(buff[i+1] == '\0')
						{
							buff = getStream(&cleanFile);
							if(buff[0] == '\0')
							{
								i = 0;
								return makeToken("ENDDEF", ">>", linenumber);
							}
							i = -1;
						}
						if(buff[i+1] == '>')
						{
							i += 2;
							return makeToken("DRIVERENDDEF", ">>>", linenumber);
						}
						i++;
						return makeToken("ENDDEF", ">>", linenumber);
					case '=':
						i+=2;
						return makeToken("GE", ">=", linenumber);
					default:
						i++;
						return makeToken("GT", ">", linenumber);
				}
			case '=': //check for == 
				if(buff[i+1] == '\0')
				{
					buff = getStream(&cleanFile);
					if(buff[0] == '\0')
					{
						i=0;
						return makeToken("ERROR_2", "=", linenumber);
					}
					i = -1;
				}
				switch(buff[i+1])
				{
					case '=':
						i+=2;
						return makeToken("EQ", "==", linenumber);
					default :
						i++;
						return makeToken("ERROR_2", "=", linenumber);
				}
			case '!': //check for != 
				if(buff[i+1] == '\0')
				{
					buff = getStream(&cleanFile);
					if(buff[0] == '\0')
					{
						i=0;
						return makeToken("ERROR_2", "!", linenumber);
					}
					i = -1;
				}
				switch(buff[i+1])
				{
					case '=':
						i+=2;
						return makeToken("NE", "!=", linenumber);
					default:
						i++;
						return makeToken("ERROR_2", "!", linenumber);
				}
			case ':': //check for :=
				if(buff[i+1] == '\0')
				{
					buff = getStream(&cleanFile);
					if(buff[0] == '\0')
					{
						i = 0;
						return makeToken("COLON", ":", linenumber);
					}
					i = -1;
				}
				switch(buff[i+1])
				{
					case '=':
						i+=2;
						return makeToken("ASSIGNOP", ":=", linenumber);
					default:
						i++;
						return makeToken("COLON", ":", linenumber);
				}
			case '.': //check for ..
				if(buff[i+1] == '\0')
				{
					buff = getStream(&cleanFile);
					if(buff[0] == '\0')
					{
						i = 0;
						return makeToken("ERROR_2", ".", linenumber);
					}
					i = -1;
				}
				switch(buff[i+1])
				{
					case '.':
						i+=2;
						return makeToken("RANGEOP", "..", linenumber);
					default:
						i++;
						return makeToken("ERROR_2", ".", linenumber);
				}
			default:
				if(isdigit(buff[i])) //check for NUM and RNUM
				{
					int lo = 0; //pinter for val char array
					val[lo++] = buff[i]; //inserting first digit scanned

					if(buff[i+1] == '\0')
					{
						buff = getStream(&cleanFile);
						if(buff[0] == '\0')
						{
							i = 0;
							val[lo] = '\0';
							return makeToken("NUM", val, linenumber);
						}
						i = -1;
					}
					while(isdigit(buff[i+1]))
					{
						i++;
						val[lo++] = buff[i];
						if(buff[i+1] == '\0')
						{
							buff = getStream(&cleanFile);
							if(buff[0] == '\0')
							{
								i = 0;
								val[lo] = '\0';
								return makeToken("NUM", val, linenumber);
							}
							i = -1;
						}
					}
					if(buff[i+1] == '.')
					{
						i++;
						val[lo++] = buff[i];
						if(buff[i+1] == '\0')
						{
							buff = getStream(&cleanFile);
							if(buff[0] == '\0')
							{
								i=0;
								val[lo] = '\0';
								return makeToken("ERROR_3", val, linenumber);
							}
							i = -1;
						}
						if(buff[i+1] == '.')
						{
							// currently on the first .
							// no need for i--
							val[lo-1] = '\0';
							return makeToken("NUM", val, linenumber);
						}
						else if(isdigit(buff[i+1]))
						{
							i++;
							val[lo++] = buff[i];
							if(buff[i+1] == '\0')
							{
								buff = getStream(&cleanFile);
								if(buff[0] == '\0')
								{
									i = 0;
									val[lo] = '\0';
									return makeToken("RNUM", val, linenumber);
								}
								i = -1;
							}
							while(isdigit(buff[i+1]))
							{
								i++;
								val[lo++] = buff[i];
								if(buff[i+1] == '\0')
								{
									buff = getStream(&cleanFile);
									if(buff[0] == '\0')
									{
										i = 0;
										val[lo] = '\0';
										return makeToken("RNUM", val, linenumber);
									}
									i = -1;
								}
							}
							if(buff[i+1] == 'e' || buff[i+1] == 'E')
							{
								i++;
								val[lo++] = buff[i];
								if(buff[i+1] == '\0')
								{
									buff = getStream(&cleanFile);
									if(buff[0] == '\0')
									{
										i = 0;
										val[lo] = '\0';
										return makeToken("ERROR_3", val, linenumber);
									}
									i = -1;
								}
								if(buff[i+1] == '+' || buff[i+1] == '-')
								{
									i++;
									val[lo++] = buff[i];
									if(buff[i+1] == '\0')
									{
										buff = getStream(&cleanFile);
										if(buff[0] == '\0')
										{
											i = 0;
											val[lo] = '\0';
											return makeToken("ERROR_3", val, linenumber);
										}
										i = -1;
									}
									if(isdigit(buff[i+1]))
									{
										i++;
										val[lo++] = buff[i];
										if(buff[i+1] == '\0')
										{
											buff = getStream(&cleanFile);
											if(buff[0] == '\0')
											{
												i = 0;
												val[lo] = '\0';
												return makeToken("RNUM", val, linenumber);
											}
											i = -1;
										}
										while(isdigit(buff[i+1]))
										{
											i++;
											val[lo++] = buff[i];
											if(buff[i+1] == '\0')
											{
												buff = getStream(&cleanFile);
												if(buff[0] == '\0')
												{
													i = 0;
													val[lo] = '\0';
													return makeToken("RNUM",val, linenumber);
												}
												i = -1;
											}
										}
										if(isdelim(buff[i+1]))
										{
											i++;
											val[lo] = '\0';
											return makeToken("RNUM",val, linenumber);
										}
										while(!isdelim(buff[i+1]))
										{
											i++;
											val[lo++] = buff[i];
											if(buff[i+1] == '\0')
											{
												buff = getStream(&cleanFile);
												if(buff[0] == '\0')
												{
													i = 0;
													val[lo] = '\0';
													return makeToken("ERROR_3", val, linenumber);
												}
												i = -1;
											}
										}
										i++;
										val[lo] = '\0';
										return makeToken("ERROR_3", val, linenumber);
									}
									else
									{
										while(!isdelim(buff[i+1]))
										{
											i++;
											val[lo++] = buff[i];
											if(buff[i+1] == '\0')
											{
												buff = getStream(&cleanFile);
												if(buff[0] == '\0')
												{
													i = 0;
													val[lo] = '\0';
													return makeToken("ERROR_3", val, linenumber);
												}
												i = -1;
											}
										}
										i++;
										val[lo] = '\0';
										return makeToken("ERROR_3", val, linenumber);
									}
								}
								else if(isdigit(buff[i+1]))
								{
									i++;
									val[lo++] = buff[i];
									if(buff[i+1] == '\0')
									{
										buff = getStream(&cleanFile);
										if(buff[0] == '\0')
										{
											i = 0;
											val[lo] = '\0';
											return makeToken("RNUM", val, linenumber);
										}
										i = -1;
									}
									while(isdigit(buff[i+1]))
									{
										i++;
										val[lo++] = buff[i];
										if(buff[i+1] == '\0')
										{
											buff = getStream(&cleanFile);
											if(buff[0] == '\0')
											{
												i = 0;
												val[lo] = '\0';
												return makeToken("RNUM", val, linenumber);
											}
											i = -1;
										}
									}
									if(isdelim(buff[i+1]))
									{
										i++;
										val[lo] = '\0';
										return makeToken("RNUM", val, linenumber);
									}
									while(!isdelim(buff[i+1]))
									{
										i++;
										val[lo++] = buff[i];
										if(buff[i+1] == '\0')
										{
											buff = getStream(&cleanFile);
											if(buff[0] == '\0')
											{
												i = 0;
												val[lo] = '\0';
												return makeToken("ERROR_3", val, linenumber);
											}
											i = -1;
										}
									}
									i++;
									val[lo] = '\0';
									return makeToken("ERROR_3", val, linenumber);
								}
								else
								{
									while(!isdelim(buff[i+1]))
									{
										i++;
										val[lo++] = buff[i];
										if(buff[i+1] == '\0')
										{
											buff = getStream(&cleanFile);
											if(buff[0] == '\0')
											{
												i = 0;
												val[lo] = '\0';
												return makeToken("ERROR_3", val, linenumber);
											}
											i = -1;
										}
									}
									i++;
									val[lo] = '\0';
									return makeToken("ERROR_3", val, linenumber);
								}
							}
							else
							{
								if(isdelim(buff[i+1]))
								{
									i++;
									val[lo] = '\0';
									return makeToken("RNUM", val, linenumber);
								}
								while(!isdelim(buff[i+1]))
								{
									i++;
									val[lo++] = buff[i];
									if(buff[i+1] == '\0')
									{
										buff = getStream(&cleanFile);
										if(buff[0] == '\0')
										{
											i = 0;
											val[lo] = '\0';
											return makeToken("ERROR_3", val, linenumber);
										}
										i = -1;
									}
								}
								i++;
								val[lo] = '\0';
								return makeToken("ERROR_3", val, linenumber);
							}
						}
						else
						{
							while(!isdelim(buff[i+1]))
							{
								i++;
								val[lo++] = buff[i];
								if(buff[i+1] == '\0')
								{
									buff = getStream(&cleanFile);
									if(buff[0] == '\0')
									{
										i = 0;
										val[lo] = '\0';
										return makeToken("ERROR_3", val, linenumber);
									}
									i = -1;
								}
							}
							i++;
							val[lo] = '\0';
							return makeToken("ERROR_3", val, linenumber);
						}			
					}
					else
					{
						if(isdelim(buff[i+1]))
						{
							i++;
							val[lo] = '\0';
							return makeToken("NUM", val, linenumber);
						}
						while(!isdelim(buff[i+1]))
						{
							i++;
							val[lo++] = buff[i];
							if(buff[i+1] == '\0')
							{
								buff = getStream(&cleanFile);
								if(buff[0] == '\0')
								{
									i = 0;
									val[lo] = '\0';
									return makeToken("ERROR_3", val, linenumber);
								}
								i = -1;
							}
						}
						i++;
						val[lo] = '\0';
						return makeToken("ERROR_3", val, linenumber);
					}
				}
				else if(isalpha(buff[i])) //check for identifier and keywords
				{
					int lo = 0;
					val[lo++] = buff[i];
					if(buff[i+1] == '\0')
					{
						buff = getStream(&cleanFile);
						if(buff[0] == '\0')
						{
							i = 0;
							val[lo] = '\0';
							tokeninfo* presentornot = presentsymboltable(table, val);
							if(presentornot)
							{
								return makeToken(presentornot->tokenname, val, linenumber);
							}
							if(lo > 8)
							{
								return makeToken("ERROR_1", val, linenumber);
							}
							insertsymboltable(table, "ID", val, linenumber);
							return makeToken("ID", val, linenumber);
						}
						i = -1;
					}
					while(buff[i+1] == '_' || isdigit(buff[i+1]) || isalpha(buff[i+1]) )
					{
						i++;
						val[lo++] = buff[i];
						if(buff[i+1] == '\0')
						{
							buff = getStream(&cleanFile);
							if(buff[0] == '\0')
							{
								i = 0;
								val[lo] = '\0';
								tokeninfo* presentornot = presentsymboltable(table, val);
								if(presentornot)
								{
									return makeToken(presentornot->tokenname, val, linenumber);
								}
								if(lo > 8)
								{
									return makeToken("ERROR_1", val, linenumber);
								}
								insertsymboltable(table, "ID", val, linenumber);
								return makeToken("ID", val, linenumber);
							}
							i = -1;
						}
					}
					i++;
					val[lo] = '\0';
					tokeninfo* presentornot = presentsymboltable(table, val);
					if(presentornot)
					{
						return makeToken(presentornot->tokenname, val, linenumber);
					}
					if(lo > 8)
					{
						return makeToken("ERROR_1", val, linenumber);
					}
					insertsymboltable(table, "ID", val, linenumber);
					return makeToken("ID", val, linenumber);
				}
				else //wrongput
				{
					char wronginput[2];
					wronginput[0] = buff[i];
					wronginput[1] = '\0';
					return makeToken("ERROR_2", wronginput, linenumber);
				}
		}
	}

	printf("###########ERROR AT END OF GETNEXTTOKEN FUNCTION#########");
	return makeToken("EOF", "EOF", linenumber);
}

tokeninfo* getAllTokens(FILE* testCaseFile) //returns list of all tokens
{
	removeComments(testCaseFile);
	FILE* cleanFile = fopen("cleanFile.txt", "rb");
	symboltable* table = makesymboltable();
	addKeywords(table);

	tokeninfo* listoftokens = NULL;
	tokeninfo* last = NULL;

	listoftokens = last = getNextToken(cleanFile, table);

	while(1)
	{
		tokeninfo* ret = getNextToken(cleanFile, table);
		if(strcmp(ret->tokenname, "EOF") == 0)
		{
			fclose(cleanFile);
			last->next = makeToken("$", "$", 0);
			return listoftokens;
		}
		last->next = ret;
		last = last->next;
	}
	fclose(cleanFile);
	last->next = makeToken("$", "$", 0);
	return listoftokens;
}
