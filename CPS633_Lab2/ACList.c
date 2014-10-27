#include "Headers.h"
//Create a new access control linked list
ACList * CreateACList(const char * filename)
{
	ACList * list = malloc(sizeof(ACList));
	list->filename = calloc(strlen(filename) + 1, sizeof(char));
	strcpy(list->filename, filename);
	list->firstNode = NULL;
	return list;
}
//create a new node in the access control linked list
ACNode * CreateACNode(const char * username, const char * permissions)
{
	ACNode * node = malloc(sizeof(ACNode));
	node->username = calloc(sizeof(username) + 1, sizeof(char));
	strcpy(node->username, username);
	node->permissions = calloc(sizeof(permissions) + 1, sizeof(char));
	strcpy(node->permissions, permissions);
	node->next = NULL;
	return node;
}
//add a node to the end of a access control linked list
void AddACNode(ACList * list, const char * username, const char * permissions)
{
	if (list == NULL) return;

	ACNode * node = CreateACNode(username, permissions);

	if (list->firstNode == NULL)
	{
		list->firstNode = node;
	}
	else
	{
		ACNode * temp = list->firstNode;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = node;
	}
}
//print out the contents of an access control linked list
void PrintACL(ACList * list)
{
	if (list == NULL) return;
	printf("%s:\n", list->filename);
	if (list->firstNode == NULL)
	{
		printf("\tNo permissions found.\n");
	}
	else
	{
		ACNode * temp = list->firstNode;
		while (temp != NULL)
		{
			printf("\t%10s %5s\n", temp->username, temp->permissions);
			temp = temp->next;
		}
	}
}