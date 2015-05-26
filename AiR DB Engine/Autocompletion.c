#include "Autocompletion.h"
/**************************************************************************************************
										  Autocompletion


**************************************************************************************************/

#include <conio.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_SIZE 48

int fBinComp_str(void* ref, void* data)
{
	if (_stricmp((char*)ref,(char*)data)<0)
		return 1;
	else
		return 0;
}

int loadDictionary(Dictionary* dict, char* filename)
{
	int count = 0;
	char* buff;
	FILE* file;
	
	fopen_s(&file, filename, "r");

	if (!file) return -1;

	buff = (char*)malloc(sizeof(char) * MAX_WORD_SIZE);

	while (fscanf_s(file, "%s", buff, MAX_WORD_SIZE) != EOF)
	{
		count++;
		BST_insert(dict, fBinComp_str, buff);
		buff = (char*)malloc(sizeof(char) * MAX_WORD_SIZE);
	}
	free(buff);

	return count;
}

void fPrintDictElement(void* data)
{
	printf("%s", (char*)data);
}

int printDictionary(Dictionary dict)
{
	return BST_traverse(dict, fPrintDictElement, BST_INORDER);
}

int ac_gets(Dictionary dict, char* buff)
{
	int i = 0, j = 0, n = 0;
	char a;
	BST_node* t;

	do
	{
		a = _getch();
		switch (a)
		{
		case '\b':
			if (!i) break;
			i--;
			printf("\b \b");
			break;
		case '\r':
			buff[i] = 0;
			printf("\n");
			break;
		case '\t':
			buff[i] = 0;
			t = dict;

			while (t)
			{
				j = 0;

				do{
					if (tolower(((char*)t->data)[j]) != tolower(buff[j+n])) break;
					j++;
				} while (((char*)t->data)[j] && (buff[j+n]));

				if (buff[j+n] == 0)
				{
					while (((char*)t->data)[j])
					{
						buff[j+n] = ((char*)t-> data)[j];
						putchar(buff[j+n]);
						j++;
					}
					buff[j+n] = 0;
					i = j+n;
					break;
				}
				else if (tolower(((char*)t->data)[j]) > tolower(buff[j+n]))
					t = t->left;
				else
					t = t->right;
			}
			break;
		case ' ':
			buff[i++] = ' ';
			n = i;
			putchar(' ');
			break;
		default:
			putchar(a);
			buff[i++] = a;
		}
	} while (a != '\r');
	return 0;
	//buff[i] = 0;
}