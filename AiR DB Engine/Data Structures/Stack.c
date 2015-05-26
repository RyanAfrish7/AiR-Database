#include <stdlib.h>

typedef struct stack_s99
{
	void* data;
	struct stack_99* prev;
} stack_node;

typedef stack_node node;

stack_node* push(stack_node** Stack, void* data)
{
	node* T = *Stack;
	*Stack = (node* )malloc(sizeof(node));
	(*Stack)->prev = T;
	(*Stack)->data = data;

	return *Stack;
}

void* pop(stack_node** Stack)
{
	void* data = NULL;
	node* T = *Stack;

	if (!*Stack) return NULL;

	(*Stack) = (*Stack)->prev;
	data = T->data;
	free(T);

	return data;
}