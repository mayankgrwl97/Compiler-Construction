#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* getStream(FILE* fp)
{
	char* testCaseFile;
	fseek(fp,0,SEEK_END);
	size_t inputsize = ftell(fp);
	rewind(fp);
	testCaseFile = (char*)malloc(inputsize * (sizeof(char)));
	fread(testCaseFile, sizeof(char), inputsize, fp);
	return testCaseFile;
}

void removeComments(char* testCaseFile, char** cleanFile)
{
	size_t inputsize = strlen(testCaseFile);
	*cleanFile = (char*)malloc(inputsize * (sizeof(char)));
	int i = 0;
	int pt = 0;
	while(i < inputsize)
	{
		if(testCaseFile[i] == '*' && i+1<inputsize && testCaseFile[i+1] == '*')
		{
			i = i+2;
			while(i+1<inputsize && (testCaseFile[i] != '*' || testCaseFile[i+1] != '*'))
				i++;
			i += 2;
		}
		else
			(*cleanFile)[pt++] = testCaseFile[i++];
	}
	(*cleanFile)[i] = '\0';
	return;
}

char* getNextToken(char* cleanFile)
{
	static int i=0;
	int l = strlen(cleanFile);
	while(i<l)
	{
		if(cleanFile[i] == ' ' || cleanFile[i] == '\t' || cleanFile[i] == '\n')
		{
			i++;
			continue;
		}
		else if(cleanFile[i] == '+')
		{
			i++;
			return "PLUS";
		}
		else if(cleanFile[i] == '-')
		{
			i++;
			return "MINUS";
		}
		else if(cleanFile[i] == '*')
		{
			i++;
			return "MUL";
		}
		else if(cleanFile[i] == '/')
		{
			i++;
			return "DIV";
		}
		else if(cleanFile[i] == '<')
		{
			if(i+1<l)
			{
				if(cleanFile[i+1] == '<')
				{
					i++;
					i++;
					return "DEF";
				}
				else if(cleanFile[i+1] == '=')
				{
					i++;
					i++;
					return "LE";
				}
				else
				{
					i++;
					return "LT";
				}
			}
			i++;
			return "LT";
		}
		else if(cleanFile[i] == '>')
		{
			if(i+1<l)
			{
				if(cleanFile[i+1] == '>')
				{
					i++;
					i++;
					return "ENDDEF";
				}
				else if(cleanFile[i+1] == '=')
				{
					i++;
					i++;
					return "GE";
				}
				else
				{
					i++;
					return "GT";
				}
			}
			i++;
			return "GT";
		}
		else if(cleanFile[i] == '=' && i+1<l && cleanFile[i+1] == '=')
		{
			i++;
			i++;
			return "EQ";
		}
		else if(cleanFile[i] == '!' && i+1<l && cleanFile[i+1] == '=')
		{
			i++;
			i++;
			return "NE";
		}
		else if(cleanFile[i] == ':')
		{
			if(i+1 < l && cleanFile[i+1] == '=')
			{
				i++;
				i++;
				return "ASSIGNOP";
			}
			i++;
			return "COLON";
		}
		else if(cleanFile[i] == '.' && i+1<l && cleanFile[i+1] == '.')
		{
			i++;
			i++;
			return "RANGEOP";
		}
		else if(cleanFile[i] == ';')
		{
			i++;
			return "SEMICOL";
		}
		else if(cleanFile[i] == ',')
		{
			i++;
			return "COMMA";
		}
		else if(cleanFile[i] == '(')
		{
			i++;
			return "BO";
		}
		else if(cleanFile[i] == ')')
		{
			i++;
			return "BC";
		}
		else if(cleanFile[i] == '[')
		{
			i++;
			return "SQBO";
		}
		else if(cleanFile[i] == ']')
		{
			i++;
			return "SQBC";
		}
		else if(isdigit(cleanFile[i]))
		{
			i++;
			while(i<l && isdigit(cleanFile[i]))
				i++;
			if(i<l && cleanFile[i] == '.')
			{
				if(i+1<l && cleanFile[i+1] == '.')
				{
					i -= 2;
					i++;
					return "NUM";
				}
				else if(i+1<l && isdigit(cleanFile[i+1]))
				{
					i++;
					while(i<l && isdigit(cleanFile[i]))
						i++;
					if(i<l && (cleanFile[i] == 'e' || cleanFile[i] == 'E'))
					{
						i++;
						if(i<l && (cleanFile[i] == '+' || cleanFile[i] == '-'))
							i++;
						if(i<l && isdigit(cleanFile[i]))
						{
							i++;
							while(i<l && isdigit(cleanFile[i]))
								i++;
							// i++;
							return "RNUM";
						}
					}
					// i++;
					return "RNUM";
				}
			}
			else
			{
				// i++;
				return "NUM";
			}
		}
		else if(isalpha(cleanFile[i]))
		{
			i++;
			while(i<l && (isalpha(cleanFile[i]) || isdigit(cleanFile[i]) || cleanFile[i] == '_'))
				i++;
			return "ID";
		}
	}
	return "ERROR";
}

int main()
{
	FILE* inputfile = fopen("test.txt","rb");
	char* testCaseFile = getStream(inputfile);
	fclose(inputfile);
	char* cleanFile;
	removeComments(testCaseFile, &cleanFile);
	printf("%s", cleanFile);
	printf("\n--*--*--*--\n");
	while(1){
		char* ret = getNextToken(cleanFile);
		if(strcmp(ret, "ERROR") == 0)
			return 0;
		printf("%s\n", ret);
	}
	return 0;
}