#pragma once

/**************************************************************************************************
												AiR Database Engine

**************************************************************************************************/

//#define __DEBUG_ENGINE

#define UNRECOGNIZED_KEYWORD	1
#define INVALID_SYNTAX			2
#define DATATYPE_MISMATCH		3
#define INVALID_LOGIN			4
#define INVALID_PARAMETER		5

#define DT_INT					1
#define DT_CHAR					2

// Configurations

#define SCRIPT_MAXLINESIZE		128

int processAiRSQL(char* BuffLine, char* errBuff);
int processAiRDB(char* filename);
