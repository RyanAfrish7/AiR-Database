#include "Data Structures.h"

#define mov_next(t)  {t = t->next;}
#define mov_prev(t)  {t = t->prev;}

typedef list_node node;

node* list_insert(list* List, int pos, void* data)
{
	node* tnode = List->front;
	
	if (pos == LIST_LASTPOS)
	{
		if (!List->end)
		{
			List->front = List->end = (node*)malloc(sizeof(node));
			List->front->data = data;
			List->front->next = List->front->prev = NULL;
			return List->front;
		}
		else
		{
			tnode = List->end;
			tnode->next = (node*)malloc(sizeof(node));
			tnode->next->data = data;
			tnode->next->next = NULL;
			tnode->next->prev = tnode;
			List->end = tnode->next;
			return tnode->next;
		}
	}
	while (pos--)
	{
		if (!tnode->next) return NULL;
		tnode = tnode->next;
	}
	if (tnode->prev)
	{
		tnode->prev->next = (node*)malloc(sizeof(node));
		tnode->prev->next->prev = tnode->prev;
		tnode->prev->next->next = tnode;
		tnode->prev = tnode->prev->next;
		tnode->prev->data = data;
		return tnode->prev;
	}
	else
	{
		List->front = (node*)malloc(sizeof(node));
		List->front->prev = NULL;
		List->front->next = tnode;
		tnode->prev = List->front;
		List->front->data = data;
		return List->front;
	}
}

void* list_delete(list* List, int pos)
{
	node* t = List->front;
	void * data;

	if (pos == LIST_LASTPOS)
	{
		t = List->end;
		List->end = t->prev;
		data = t->data;
		free(t);
		if (List->end)
			List->end->next = NULL;
		else
			List->front = NULL;
		return data;
	}

	t = List->front;

	if (pos == 0)
	{
		data = t->data;
		mov_next(t);
		if(t) t->prev = NULL;
		free(List->front);
		List->front = t;
		if (t == NULL)
			List->end = NULL;
		return data;
	}

	while ((t->next) && (pos--))
		mov_next(t);

	if (pos == 0 || pos == -1)
	{
		//t->prev->next = t->next;
		if (t->next)
		{
			t->next->prev = t->prev;
		}
		else
		{
			List->end = List->end->prev;
		}
		if (t->prev)
		{
			t->prev->next = t->next;
		}
		data = t->data;
		free(t);

		return data;
	}

	return NULL;
}