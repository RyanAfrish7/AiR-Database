#pragma once

#include <stdlib.h>
#include <ctype.h>

//*************************************************************************************************
// Queue

typedef struct queue_s99
{
	void* data;
	struct queue_s99* prev;
	struct queue_s99* next;
} queue_node;

typedef struct queue_s98{
	queue_node* start;
	queue_node* end;
} queue;

queue_node* enqueue(queue* Queue, void* data);
void* dequeue(queue* Queue);

//*************************************************************************************************
// Binary Search Tree - BST

#define BST_PREORDER -1
#define BST_INORDER   0
#define BST_POSTORDER 1

typedef struct BST_s99
{
	void* data;
	struct BST_s99* left;
	struct BST_s99* right;
} BST_node;
 
// fBinaryCompare function must be defined by the user to compare two data and return 1 for the 
// node to be right node and 0 for the node to be left node. Returning any other values may produce 
// unexpected results. Most commonly used fBinComp function for integer is provided below which  
// may be used as a template.

typedef int(*fBinaryCompare)(void* ref, void* data);
typedef void(*fProcessElement)(void* data);

int fBinComp_int(void* ref, void* data);

int BST_insert(BST_node** root, fBinaryCompare fBinComp, void* data);
int BST_traverse(BST_node* root, fProcessElement fProcElmt, int mode);

//*************************************************************************************************
// Linked List

#define LIST_LASTPOS -1

typedef struct list_s99
{
	void* data;
	struct list_s99* next, *prev;
} list_node;

typedef struct list_s98
{
	list_node* front, *end;
} list;

list_node* list_insert(list* List, int pos, void* data);
void* list_delete(list* List, int pos);