#pragma once

/**************************************************************************************************
											Autocompletion


**************************************************************************************************/

#include "Data Structures\Data Structures.h"

typedef BST_node* Dictionary;

int loadDictionary(Dictionary* dict, char* filename);

#if defined __DEBUG
int printDictionary(Dictionary dict);
#endif

int ac_gets(Dictionary dict, char* buff);