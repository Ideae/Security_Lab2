#include "Headers.h"



ACList * CreateACList(const char * filename)
{
	ACList * list = malloc(sizeof(ACList));
	//check later if the +1 is necessary
	list->filename = calloc(strlen(filename) + 1, sizeof(char));
	strcpy(list->filename, filename);
	list->firstNode = NULL;
	return list;
}
void AddACNode(ACList * list, const char * username, const char * permissions)
{
	if (list == NULL) return;

	ACNode * node = malloc(sizeof(ACNode));
	node->username = calloc(sizeof(username) + 1, sizeof(char));
	strcpy(node->username, username);
	node->permissions = calloc(sizeof(permissions) + 1, sizeof(char));
	strcpy(node->permissions, permissions);
	node->next = NULL;

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

void PrintACL(ACList * list)
{
	if (list == NULL) return;
	printf("ACL for the file: %s:\n", list->filename);
	if (list->firstNode == NULL)
	{
		printf("\tNo permissions found.\n");
	}
	else
	{
		ACNode * temp = list->firstNode;
		while (temp != NULL)
		{
			printf("\tUsername:%10s Permissions:%5s\n", temp->username, temp->permissions);
			temp = temp->next;
		}
	}

}