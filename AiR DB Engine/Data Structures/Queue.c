#include "Data Structures.h"

#define prev(a) {a = a->prev;}
#define next(a) {a = a->next;}

typedef queue_node node;

queue_node* enqueue(queue* Queue, void* data)
{
	if (Queue->end)
	{
		Queue->end->next = (node*)malloc(sizeof(node));
		Queue->end->next->next = NULL;
		Queue->end->next->prev = Queue->end;
		Queue->end->next->data = data;
		next(Queue->end);
	}
	else
	{
		Queue->start = Queue->end = (node*)malloc(sizeof(node));
		Queue->end->next = NULL;
		Queue->end->prev = NULL;
		Queue->end->data = data;
	}
	return Queue->end;
}

void* dequeue(queue* Queue)
{
	if (Queue->start)
	{
		void* data = Queue->start->data;

		if (Queue->start == Queue->end)
		{
			free(Queue->start);
			Queue->start = Queue->end = NULL;
		}
		else
		{
			next(Queue->start);
			free(Queue->start->prev);
			Queue->start->prev = NULL;
		}
		return data;
	}
	else
		return NULL;
}