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

int mainAuthentication();
int mainAuthorization(char * username);
int mainEditMatrix();
int mainRBAC(char * username);
int mainLogin(char * username);

void PopulateUserData();
void InitOKAM(int userNumber, double threshold);
int AttemptLogin(int userNumber, int verifySet, double threshold);

void DeletePermissions(ACList * list, const char * username);
void AddPermissions(ACList * list, const char * username, const char * permissions);
void EditPermissions(ACList * list, const char * username, const char * permissions);
void WriteMatrixFile();
void EditAccessMatrix(const char * username, const char * filename, const char * permissions);

ACList * CreateACList(const char * filename);
ACNode * CreateACNode(const char * username, const char * permissions);
void AddACNode(ACList * list, const char * username, const char * permissions);
void PrintACL(ACList * list);
void TestACList();
void AccessRequest(const char * username, const char * filename, const char * permissions);
void PrintACLists();
void ReadAccessControlMatrix();

void InitRBAC();
void SetRole(char * username, char * newrole);
void RequestRBAC(const char * username, const char * filename, const char * permissions);