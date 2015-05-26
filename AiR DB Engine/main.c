#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
//#define __DEBUG
#include "AiR DB Engine.h"
#include "Autocompletion.h"



int main(int argc, char* argv[])
{
	int op;
	char buff[512], errBuff[512];
	Dictionary dict = NULL;

	loadDictionary(&dict, "Dictionary.dat");

	printf("AiRSQL mode >> 1\nAiRDB  mode >> 2\n");
	printf("Choose mode >> ");
	scanf("%d", &op);

	if (op == 2)
	{
		printf("\nFilepath : ");
		scanf("%s",buff);
		processAiRDB(buff);
	}
	else if (op == 1)
	{
		printf("Enter the string : ");

		while (1)
		{
			printf("\n[AiRSQL]> ");
			ac_gets(dict, buff);
			if (buff[0] == '0') return 0;
			int a = processAiRSQL(buff, errBuff);
			switch (a)
			{
			case DATATYPE_MISMATCH:
				printf("%s\n", errBuff);
				goto breakloop;
			default:
				break;
			}
			printf("%s\n", errBuff);
		}
	}
breakloop:

	printf("Press any key to continue...");
	_getch();
	return 0;
}