#include "AiR DB Engine.h"

/**************************************************************************************************
												AiR Database Engine
												
													  v0.3

**************************************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include "Data Structures/Data Structures.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>

#define errBuff_s99 errBuff
#define set_err(txt) {if(errBuff_s99) strcpy_s(errBuff_s99, 512, txt);}

typedef struct
{
	char* name;
	int datatype;
	int size;
} column;

typedef struct
{
	char* name;
	int ncols;
	list cols;
	list records;
} table;

typedef struct
{
	char username[32];
	char password[64];
}user;

typedef struct 
{
	char* name;
	queue script;
	queue sql;
	queue* tsql;
} menuentry;

int parse(FILE* file, queue* script, queue* sql);
int handleScript(FILE* file, char* BuffLine);
void copysql(queue** tsql, queue* sql);
int getPassword(char* Buff);
int executescript(queue* script, queue* sql, queue* tsql);
char *strrep(char *orig, char *rep, char *with);
int printMenu();

table* Table;
list* UserList;
list* Menu;

int printMenu()
{
	int i = 0;
	list_node* menuNode = Menu->front;
	//printf("%d. %s", i++, "Log");
	while (menuNode)
	{
		menuentry* menuEntry = (menuentry*)menuNode->data;
		printf("%d. %s\n", i++, menuEntry->name);
		menuNode = menuNode->next;
	}

	printf("\n");
}

char *strrep(char *orig, char *rep, char *with)
{
	char *result;
	char *ins;
	char *tmp;
	int len_rep;
	int len_with;
	int len_front;
	int count;

	if (!orig) return NULL;
	
	if (!rep) rep = "";
	len_rep = strlen(rep);
	
	if (!with) with = "";
	len_with = strlen(with);

	ins = orig;

	for (count = 0; tmp = strstr(ins, rep); ++count)
	{
		ins = tmp + len_rep;
	}
	char* ttmp;
	int t;
	tmp = result = malloc(SCRIPT_MAXLINESIZE);
	ttmp = tmp;
	if (!result) return NULL;

	while (count--) {
		ins = strstr(orig, rep);
		len_front = ins - orig;
		tmp = strncpy(tmp, orig, len_front) + len_front;
		tmp = strcpy(tmp,  with) + len_with;
		orig += len_front + len_rep;
	}
	
	strcpy(tmp, orig);
	return result;
}

int parse(FILE* file, queue* script, queue* sql)
{
	int once = -1;
	char* buff;
again:
	buff = (char*)malloc(sizeof(char) * SCRIPT_MAXLINESIZE);
	fgets(buff, SCRIPT_MAXLINESIZE, file);
	buff[strlen(buff) - 1] = 0;
	if (buff == 0) return 0;
	if (!_stricmp(buff, "SQL{"))
	{
		while (1)
		{
			char* nbuff = (char*)malloc(sizeof(char) * SCRIPT_MAXLINESIZE);
			fgets(nbuff, SCRIPT_MAXLINESIZE, file);
			nbuff[strlen(nbuff) - 1] = 0;
			if (!_stricmp(nbuff, "}"))
				break;
			enqueue(sql, nbuff);
		}
	}
	else if (!_stricmp(buff, "SCRIPT{"))
	{
		while (1)
		{
			char* nbuff = (char*)malloc(sizeof(char) * SCRIPT_MAXLINESIZE);
			fgets(nbuff, SCRIPT_MAXLINESIZE, file);
			nbuff[strlen(nbuff) - 1] = 0;
			if (!_stricmp(nbuff, "}"))
				break;
			enqueue(script,nbuff);
		}
	}
	if (once++)
	goto again;
	return 0;
}

void copysql(queue** tsql, queue* sql)
{
	queue_node* qnode = sql->start;
	*tsql = malloc(sizeof(queue));
	(*tsql)->start = (*tsql)->end = NULL;
	while (qnode)
	{
		void* data = malloc(SCRIPT_MAXLINESIZE);
		strcpy_s(data, SCRIPT_MAXLINESIZE, qnode->data);
		enqueue(*tsql, data);
		qnode = qnode->next;
	}
}

int executescript(queue* script, queue* sql, queue* tsql)
{
	int safe_s99 = -1;
	int a= 0;
	queue_node* sline = script->start;
	queue_node* sqlline = sql->start;
	char str[SCRIPT_MAXLINESIZE];

	while (sline)
	{
		char* s = str;
		char* tok = NULL;
		strcpy_s(s, SCRIPT_MAXLINESIZE, (char*)sline->data);

		s = strtok_s(s, " ", &tok);
		
		if (!_stricmp(s, "GET"))
		{
			char* varname;
			int vartype;
			varname = strtok_s(NULL, " ", &tok);
			tok[strlen(tok) - 2] = 0;
			tok++;
			printf("%s", tok);
			char val[64];
			scanf_s("%s", val, 64);
			if (safe_s99)
			{
				safe_s99++;
				copysql(&tsql, sql);
			}
			sqlline = tsql->start;
			while (sqlline)
			{
				char* bn = strrep(sqlline->data, varname, val);
				strcpy_s(sqlline->data,SCRIPT_MAXLINESIZE, bn);
				sqlline = sqlline->next;
			}
		}
		else if (!_stricmp(s, "PUT"))
		{
			tok[strlen(tok) - 2] = 0;
			tok++;
			printf("%s\n", tok);
		}
		else if (!_stricmp(s, "EXEC"))
		{
			s = strtok_s(NULL, " ", &tok);
			if (!_stricmp(s, "SQL;"))
			{
				sqlline = (tsql)?tsql->start:sql->start;
				while (sqlline)
				{
					char errBuff[1024];
					processAiRSQL((char*)sqlline->data,errBuff);
					printf("%s\n", errBuff);
					sqlline = sqlline->next;
				}
				 a = 121;//unload();<<work left here
			}
		}
		else if (!_stricmp(s, "EXIT;"))
		{
			exit(0);
		}
		else if (!_stricmp(s,"MENU"))
		{
			printMenu();
		}
		sline = sline->next;
	}
	return a;
}

int getPassword(char* Buff)
{
	int i = 0;
	int n = 0;
	while ((Buff[i] = _getch()) != '\r')
	{
		switch (Buff[i])
		{
		case '\b':
			if (n == 0) continue;
			n--;
			i-=2;
			printf("\b \b");
			break;
		case '\r':
			putchar('\n');

		case ' ':
			break;
		default:
			if (isgraph((unsigned char)Buff[i]))
				putchar('*');
			n++;
			break;
		}
		i++;
	}
	Buff[i] = 0;

	return i-1;
}

int processAiRDB(char* filename)
{
	int lineno = 1, p;
	char* Buff = malloc(sizeof(char) * SCRIPT_MAXLINESIZE);
	FILE* file;
	fopen_s(&file, filename, "r");

	if (!file)
	{
		printf("Invalid File name/path");
		return INVALID_PARAMETER;
	}

	Menu = (list*)malloc(sizeof(list));
	Menu->front = Menu->end = NULL;

	fgets(Buff, SCRIPT_MAXLINESIZE, file);
	Buff[strlen(Buff) - 1] = 0;

	if ((!_stricmp("!AiRscript v0.1", Buff)))
	{
		printf("\n\nAiR Database Engine is starting");

#ifndef __DEBUG_ENGINE
		for (p = 0; p < 200000000; p++); printf(".");
		for (p = 0; p < 400000000; p++); printf(".");
		for (p = 0; p < 300000000; p++); printf(".\n");
#endif

		while (fgets(Buff, SCRIPT_MAXLINESIZE, file) != NULL)
		{
			Buff[strlen(Buff) - 1] = 0;
			lineno++;

			handleScript(file, Buff);
		}
	}

	int menuvalue,xa;
	while (1)
	{
		printf("\n[DB]> ");
		scanf_s("%d", &menuvalue);
		list_node* me = Menu->front;
		menuentry* mentry;
		xa = menuvalue;
		while (xa--)
		{
			me = me->next;
		}

		mentry = (menuentry*)me->data;
		if(executescript(&mentry->script, &mentry->sql, mentry->tsql)==121) mentry->tsql = NULL; // Memory Leakage
	}

	fclose(file);
	return 0;
}

int handleScript(FILE* file, char* BuffLine)
{
	char* tok = NULL, *a, *b = NULL;
	
	a = strtok_s(BuffLine, " ", &tok);
	if (a == NULL) return 0;

	if (!_stricmp(a, "login"))
	{
		UserList = (list*)malloc(sizeof(list));
		UserList->front = UserList->end = NULL;

		while (a = strtok_s(NULL, " ", &tok))
		{
			a[strlen(a) - 1] = 0;
			a++;

			a = strtok_s(a, ",", &b);
			user* User = (user*)malloc(sizeof(user));
			strcpy_s(User->username, _countof(User->username), a);
			strcpy_s(User->password, _countof(User->password), b);

			list_insert(UserList, LIST_LASTPOS, User);
		}

		int i = 0;
		a = malloc(sizeof(char) * 32);
		b = malloc(sizeof(char) * 64);
		printf("Please enter your credentials.\n");
		printf("Username :");
		scanf_s("%s", a, 32);
		printf("Password :");
		getPassword(b);
		putchar('\n');
		list_node* UserNode = UserList->front;
		while (UserNode)
		{
			if (!strcmp(((user*)UserNode->data)->username, a))
				if (!strcmp(((user*)UserNode->data)->password, b))
					break;
				else
				{
					printf("\n Invalid Username / Password. Relaunch the application to try again.");
					exit(0);
				}
			UserNode = UserNode->next;
		}
		if (!UserNode)
		{
			printf("\n Invalid Username / Password. Relaunch the application to try again.");
			return INVALID_LOGIN;
		}
		queue SQL, Script;
		SQL.start = SQL.end = Script.start = Script.end = NULL;
		parse(file, &Script, &SQL);
		executescript(&Script, &SQL, NULL); // no get in login <<< workedaround
	}
	else if (!_stricmp(a,"menuentry"))
	{
		int i;
		a = strtok_s(NULL, "\"", &tok);
		if (a == NULL) return;
		menuentry* t = (menuentry*)malloc(sizeof(menuentry));
		
		t->name = malloc(sizeof(char)*(i = strlen(a)+1));
		strcpy_s(t->name, i, a);
		t->script.start = t->script.end = NULL;
		t->sql.start = t->sql.end = NULL;
		t->tsql = t->tsql = NULL;
		parse(file, &t->script, &t->sql);
		list_insert(Menu, LIST_LASTPOS, t);
	}
}

int processAiRSQL(char* BuffLine, char* errBuff)
{
	int sz;
	char* a;
	char* tok = NULL;
	
	a = strtok_s(BuffLine, " ", &tok);

	if (!_stricmp(BuffLine,"CREATE"))
	{
		a = strtok_s(NULL, " ", &tok);
		
		if (!a)
		{
			set_err("[DB]: CREATE [?]what");
			return INVALID_SYNTAX;
		}

		if (!_stricmp(a, "TABLE"))
		{
			a = strtok_s(NULL, " ", &tok);
			
			if (!a)
			{
				set_err("[DB]: CREATE TABLE [?]table_name");
				return INVALID_SYNTAX;
			}

			Table = (table*)malloc(sizeof(table));
			Table->name = (char*)malloc(sizeof(char) * (sz = strlen(a) + 1));
			strcpy_s(Table->name, sz, a);
			Table->ncols = 0;
			Table->cols.front = Table->cols.end = NULL;
			Table->records.front = Table->records.end = NULL;

			a = tok;
			
			if (!a)
			{
				set_err("[DB]: CREATE TABLE table_name [!](colname coltype,...);");
				return INVALID_SYNTAX;
			}

			if (a[0] != '(')
			{
				set_err("[DB]: CREATE TABLE table_name [!](colname coltype,...);");
				return INVALID_SYNTAX;
			}
			tok = NULL;
			a[strlen(a) - 2] = 0;
			a++;
			a = strtok_s(a, ",", &tok);	
			do 
			{
				char* col, *ctok = NULL;
				column* Column = (column*) malloc(sizeof(column));
				col = strtok_s(a, " ", &ctok);
				Column->name = (char*)malloc(sizeof(char)*(sz = strlen(col)+1));
				strcpy_s(Column->name, sz, col);
				col = strtok_s(NULL," ",&ctok);
				if (!_stricmp(col, "INT"))
				{
					Column->datatype = DT_INT;
					Column->size = 4;
				}
				else if (!_stricmp(col, "CHAR"))
				{
					char* x, *xtok = NULL;

					col = strtok_s(NULL, " ", &ctok);

					Column->datatype = DT_CHAR;
					Column->size = col?atoi(col):1;
				}
				list_insert(&Table->cols, LIST_LASTPOS, Column);
			} while (a = strtok_s(NULL, ",", &tok));
		}
		set_err("");
	}
	else if (!_stricmp(BuffLine,"INSERT"))
	{
		a = strtok_s(NULL, " ", &tok);
		if (a[0] != '(')
		{
			set_err("INSERT (DATA,...)");
			return INVALID_SYNTAX;
		}
		a[strlen(a) - 2] = 0;
		a++;
		tok = NULL;

		list_node* Column = Table->cols.front;
		void* FieldData = NULL;
		list* Record = list_insert(&Table->records, LIST_LASTPOS, (list*)malloc(sizeof(list)))->data;

		Record->front = Record->end = NULL;
	
		a = strtok_s(a, ",", &tok);
		do 
		{
			switch (((column*)Column->data)->datatype)
			{
			case DT_INT:
				FieldData = (int*)malloc(sizeof(int));
				*((int*)FieldData) = atoi(a);
				break;
			case DT_CHAR:
				FieldData = (char*)malloc(sizeof(char)*((column*)Column->data)->size);
				if (strlen(a) > ((column*)Column->data)->size)
				{
					set_err("Exceeded Specified width!! Aborting...");
					return DATATYPE_MISMATCH;
				}
				strcpy_s(FieldData, ((column*)Column->data)->size, a);
				break;
			}
			list_insert(Record, LIST_LASTPOS, FieldData);
			Column = Column->next;
		} while ((a = strtok_s(NULL, ",", &tok)) && Column);
		set_err("");
	}
	else if (!_stricmp(BuffLine, "DISPLAY"))
	{
		list_node* record = Table->records.front;
		list_node* col = Table->cols.front;

		int ta,tb = 0;
		while (col)
		{
			ta = printf("%s\t",((column*)col->data)->name) - 1;
			tb += ta + 8 - (ta % 8);
			col = col->next;
		}
		tb += ta % 8 - 8;
		printf("\n");
		while (tb--)
			putchar('-');
		printf("\n");
		while (record)
		{
			list_node* FieldNode = ((list*)record->data)->front;
			col = Table->cols.front;
			while (col)
			{
				switch (((column*)col->data)->datatype)
				{
				case DT_INT:
					printf("%d\t", *((int*)FieldNode->data));
					break;
				case DT_CHAR:
					printf("%s\t", ((char*)FieldNode->data));
					break;
				default:
					break;
				}
				col = col->next;
				FieldNode = FieldNode->next;
			}
			record = record->next;
			printf("\n");
		}
		set_err("");
	}
	else if (!_stricmp(BuffLine, "DELETE"))
	{
		a = strtok_s(NULL, " ", &tok);
		if (!a)
		{
			set_err("DELETE [?](column_name=value)");
			return INVALID_SYNTAX;
		}
		if (a[0] != '(')
		{
			set_err("DELETE [?](column_name=value)");
			return INVALID_SYNTAX;
		}
		a[strlen(a) - 2] = 0;
		a++;
		tok = NULL;
		a = strtok_s(a, "=", &tok);
		sz = 0;
		list_node* tnode = Table->cols.front;
		while (1)
		{
			if (!_stricmp(((column*)tnode->data)->name, a)) break;
			if (!(tnode = tnode->next))
			{
				set_err("Invalid column name");
				return 0;
			}
			sz++;
		}
		a = strtok_s(NULL, " ", &tok);
		if (!a)
		{
			set_err("DELETE (column_name=[?]value);");
			return INVALID_SYNTAX;
		}
		int flag = 0;
		int pos = 0;
		list_node* Record = Table->records.front;
		while (Record)
		{
			int h = sz;
			list_node* FieldNode = ((list*)Record->data)->front;
			while (h--)
			{
				FieldNode = FieldNode->next;
			}
			switch (((column*)tnode->data)->datatype)
			{
			case DT_INT:
				if ((*(int*)FieldNode->data) == atoi(a))
				{
					flag = 1;
				}
				break;
			case DT_CHAR:
				if (!_stricmp((char*)FieldNode->data, a))
					flag = 1;
				break;
			default:
				break;
			}

			if (flag)
			{
				FieldNode = ((list*)Record->data)->front;
				while (FieldNode)
				{
					free(FieldNode->data);
					FieldNode->data = NULL;
					FieldNode = FieldNode->next;
				}
				list_delete(&Table->records, pos);
				set_err("");
				return 0;
			}
			Record = Record->next;
			pos++;
		}
		set_err("No Records matched");
	}
	else
	{
		printf("\n%s", BuffLine);
		set_err("[DB]: [!!]unrecognized");
		return UNRECOGNIZED_KEYWORD;
	}
}