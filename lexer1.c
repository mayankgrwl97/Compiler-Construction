#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* getStream(FILE** fp)
{
	char* testCaseFile;
	size_t inputsize = 100;
	testCaseFile = (char*)malloc(inputsize * (sizeof(char)));
	fread(testCaseFile, sizeof(char), inputsize, *fp);
	return testCaseFile;
}

void removeComments(FILE* fp)
{
	FILE* cleanFile = fopen("cleanFile.txt", "w+");//change to append
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

typedef struct tokeninfo tokeninfo;

struct tokeninfo
{
	char* tokenname;
	int linenumber;
	char* value;
};

tokeninfo* makeToken(char* tokenname, int linenumber)
{
	tokeninfo* temp = (tokeninfo*)malloc(sizeof(tokeninfo));
	temp->tokenname = tokenname;
	temp->linenumber = linenumber;
	temp->value = NULL;
	return temp;
}

tokeninfo* getNextToken(FILE* cleanFile)
{
	static int i = 0;
	static int linenumber = 1;
	static char* buff = getStream(&cleanFile);

label:

	if(buff[i] == '\0')
	{
		buff = getStream(&cleanFile);
		i = 0;
		if(buff[0] == '\0')
			return makeToken(NULL,linenumber);
	}

	switch(buff[i])
	{
		case ' ':
		case '\t':
			i++;
			goto label;
		case '\n':
			linenumber++;
			i++;
			goto label;
		case '+':
			i++;
			return makeToken("PLUS", linenumber);
		case '-':
			i++;
			return makeToken("MINUS", linenumber);
		case '*':
			i++;
			return makeToken("MUL", linenumber);
		case '/':
			i++;
			return makeToken("DIV", linenumber);
		case ';':
			i++;
			return makeToken("SEMICOL", linenumber);
		case ',':
			i++;
			return makeToken("COMMA", linenumber);
		case '(':
			i++;
			return makeToken("BO", linenumber);
		case ')':
			i++;
			return makeToken("BC", linenumber);
		case '[':
			i++;
			return makeToken("SQBO", linenumber);
		case ']':
			i++;
			return makeToken("SQBC", linenumber);
		case '<':
			if(buff[i+1] == '\0')
			{
				buff = getStream(&cleanFile);
				if(buff[0] == '\0')
					return makeToken(NULL, linenumber);
				i = -1;
			}
			switch(buff[i+1])
			{
				case '<':
					i+=2;
					return makeToken("DEF", linenumber);
				case '=':
					i+=2;
					return makeToken("LE", linenumber);
				default:
					i++;
					return makeToken("LT", linenumber);
			}
		case '>':
			if(buff[i+1] == '\0')
			{
				buff = getStream(&cleanFile);
				if(buff[0] == '\0')
					return makeToken(NULL, linenumber);
				i = -1;
			}
			switch(buff[i+1])
			{
				case '>':
					i+=2;
					return makeToken("ENDDEF", linenumber);
				case '=':
					i+=2;
					return makeToken("GE", linenumber);
				default:
					i++;
					return makeToken("GT", linenumber);
			}
		case '=':
			if(buff[i+1] == '\0')
			{
				buff = getStream(&cleanFile);
				if(buff[0] == '\0')
					return makeToken(NULL, linenumber);
				i = -1;
			}
			switch(buff[i+1])
			{
				case '=':
					i+=2;
					return makeToken("EQ", linenumber);
			}
		case '!':
			if(buff[i+1] == '\0')
			{
				buff = getStream(&cleanFile);
				if(buff[0] == '\0')
					return makeToken(NULL, linenumber);
				i = -1;
			}
			switch(buff[i+1])
			{
				case '=':
					i+=2;
					return makeToken("NE", linenumber);
			}
		case ':':
			if(buff[i+1] == '\0')
			{
				buff = getStream(&cleanFile);
				if(buff[0] == '\0')
					return makeToken(NULL, linenumber);
				i = -1;
			}
			switch(buff[i+1])
			{
				case '=':
					i+=2;
					return makeToken("ASSIGNOP", linenumber);
				default:
					i++;
					return makeToken("COLON", linenumber);
			}
		case '.':
			if(buff[i+1] == '\0')
			{
				buff = getStream(&cleanFile);
				if(buff[0] == '\0')
					return makeToken(NULL, linenumber);
				i = -1;
			}
			switch(buff[i+1])
			{
				case '.':
					i+=2;
					return makeToken("RANGEOP", linenumber);
			}
		default:
			if(isdigit(cleanFile[i])) //check
			{
				i++;
				if(buff[i+1] == '\0')
				{
					buff = getStream(&cleanFile);
					if(buff[0] == '\0')
						return makeToken(NULL, linenumber);
					i = -1;
				}

			}
	}

	return makeToken();
}

int main()
{
	FILE* inputfile = fopen("test.txt","rb");
	removeComments(inputfile);
	fclose(inputfile);
	FILE* cleanFile = fopen("cleanFile.txt", "rb");

	getNextToken(cleanFile);
	getNextToken(cleanFile);
	getNextToken(cleanFile);
	fclose(cleanFile);
	return 0;
}