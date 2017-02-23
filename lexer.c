#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* getStream(FILE* fp)
{
	char* testCaseFile;
	fseek(fp,0,SEEK_END);
	size_t inputsize = ftell(fp);
	rewind(fp);
	testCaseFile = malloc(inputsize * (sizeof(char)));
	fread(testCaseFile, sizeof(char), inputsize, fp);
	return testCaseFile;
}

void removeComments(char* testCaseFile, char* cleanFile)
{
	size_t inputsize = strlen(testCaseFile);
	cleanFile = malloc(inputsize * (sizeof(char)));
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
			cleanFile[pt++] = testCaseFile[i++];
	}
	cleanFile[i] = '\0';
	printf("%d\t%d\n", strlen(cleanFile), inputsize);
	printf("%s\n", cleanFile);
	return;
}



int main()
{
	FILE* inputfile = fopen("test.txt","rb");
	char* testCaseFile = getStream(inputfile);
	printf("%s\n", testCaseFile);
	fclose(inputfile);
	char* cleanFile;
	removeComments(testCaseFile, cleanFile);

	return 0;
}