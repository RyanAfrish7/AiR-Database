#include "Data Structures.h"

typedef BST_node node;

int fBinComp_int(void* ref, void* data)
{
	if (*(int*)ref <= *(int*)data)
		return 1;
	else
		return 0;
}

int BST_insert(BST_node** root, fBinaryCompare fBinComp, void* data)
{
	if (*root)
	{
		if (!fBinComp((*root)->data, data))
			return BST_insert(&(*root)->left, fBinComp, data) + 1;
		else
			return BST_insert(&(*root)->right, fBinComp, data) + 1;
	}else
	{
		*root = (node*)malloc(sizeof(node));
		(*root)->data = data;
		(*root)->left = NULL;
		(*root)->right = NULL;
		return 1;
	}
}

int BST_traverse(BST_node* root, fProcessElement fProcElmt, int mode)
{
	if (root)
	{
		switch (mode)
		{
		case BST_PREORDER:
			fProcElmt(root->data);
			if (root->left) BST_traverse(root->left, fProcElmt, mode);
			if (root->right) BST_traverse(root->right, fProcElmt, mode);
			break;

		case BST_INORDER:
			if (root->left) BST_traverse(root->left, fProcElmt, mode);
			fProcElmt(root->data);
			if (root->right) BST_traverse(root->right, fProcElmt, mode);
			break;

		case BST_POSTORDER:
			if (root->left) BST_traverse(root->left, fProcElmt, mode);
			if (root->right) BST_traverse(root->right, fProcElmt, mode);
			fProcElmt(root->data);
			break;

		default:
			return -1;
		}
	}
	return 0;
}