#include "Data Structures.h"

int isOperator(char op)
{
	if (op == '+' || op == '-' || op == '*' || op == '/')
		return 1;
	return 0;
}

int getPriority(char op)
{
	switch (op)
	{
	case '+':
	case '-':
		return 3;
	case '*':
	case '/':
		return 2;
	case '(':
		return 4;
	}
	return 0;
}

int RPN(char* Buff, char* OutBuff)
{
	int i = 0, j = 0, c = 0;
	stack_node* stack = NULL;

	while (Buff[i])
	{
		if (isalpha(Buff[i]))
		{
			OutBuff[j++] = Buff[i];
			c++;
		}
		else if (isOperator(Buff[i]))
		{
			while((stack) && (getPriority(Buff[i]) >= getPriority(*((char* )(stack->data)))))
				OutBuff[j++] = *((char*)pop(&stack));
			push(&stack, &Buff[i]);
		} else if (Buff[i]=='(')
			push(&stack, &Buff[i]);
		else if (Buff[i] == ')')
		{
			while (stack)
				if (*((char*)(stack->data)) == '(') { pop(&stack); break; }
				else OutBuff[j++] = *((char*)(pop(&stack)));
		}
		i++;
	}
	while (stack) 
		OutBuff[j++] = *((char*)(pop(&stack)));
	OutBuff[j] = 0;
	return j;
}

int evaluateRPN(const char* Buff, int* value)
{
	int t, o,* x;
	int i = 0, j = 0;
	stack_node* stack = NULL;
	while (Buff[i])
	{
		if (isalpha(Buff[i]))
			push(&stack, &value[j++]);
		else if (isOperator(Buff[i]))
		{
			switch (Buff[i])
			{
			case '+':
				t = *(int*)pop(&stack) + *(int*)pop(&stack);
				x = malloc(sizeof(int));
				*x = t;
				push(&stack, x);
				break;

			case '-':
				t = - *(int*)pop(&stack) + *(int*)pop(&stack);
				x = malloc(sizeof(int));
				*x = t;
				push(&stack, x);
				break;
			case '*':
				t = *(int*)pop(&stack) * *(int*)pop(&stack);
				x = malloc(sizeof(int));
				*x = t;
				push(&stack, x);
				break;
			case '/':
				t = *(int*)pop(&stack);
				o = *(int*)pop(&stack);
				t = o / t;
				x = malloc(sizeof(int));
				*x = t;
				push(&stack, x);
				break;
			}
		}
		i++;
	}
	return *(int* )pop(&stack);
}