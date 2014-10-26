//Visual Studio flags:
#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable:4996)

//Includes:
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


typedef struct ACNode{
	char * username;
	char * permissions;
	struct ACNode * next;
} ACNode;

typedef struct ACList{
	char * filename;
	ACNode * firstNode;
} ACList;


ACList * CreateACList(const char * filename);
void AddACNode(ACList * list, const char * username, const char * permissions);
void PrintACL(ACList * list);
void TestACList();
void AccessRequest(const char * username, const char * filename, const char * permissions);
void PrintACLists();
void CreateAccessControlMatrix();