#include "Headers.h"
//the access control linked lists for each different file in the system
ACList* fileLists[6];
//the matrix that contains all of the permissions in the system (each user and file)
char* matrix[5][6];
//the main loop the lets the user query the authorization system using the ACL implementation
int mainAuthorization(char * username)
{
	char * filename = calloc(8, sizeof(char));
	char * permissions = calloc(8, sizeof(char));
	while (1)
	{
		printf("Please enter access request as follows (0 0 to exit):\nfilename permissions\n");
		scanf("%s %s", filename, permissions);
		if (filename[0] == '0' && permissions[0] == '0')
		{
			return 0;
		}
		AccessRequest(username, filename, permissions);
	}
	return 0;
}
//the main input loop that lets the user edit the matrix of the permissions
int mainEditMatrix()
{
	char * filename = calloc(8, sizeof(char));
	char * username = calloc(8, sizeof(char));
	char * permissions = calloc(8, sizeof(char));
	while (1)
	{
		printf("Please enter a new permission or edit an existing one with (0 0 0 to exit):\nfilename username permissions\n");
		scanf("%s %s %s", filename, username, permissions);
		if (filename[0] == '0' && username[0] == '0' && permissions[0] == '0')
		{
			return 0;
		}
		EditAccessMatrix(username, filename, permissions);
	}
	return 0;
}
//prints out the contents of every access control list
void PrintACLists()
{
	for (int i = 0; i < 6; i++)
	{
		PrintACL(fileLists[i]);
	}
}
//reads in the access control matrix from the file and creates the access control lists accordingly
void ReadAccessControlMatrix()
{
	for (int i = 0; i < 6; i++)
	{
		char name[7];
		sprintf(name, "file%d", (i+1));
		fileLists[i] = CreateACList(name);
	}

	FILE * file = fopen("matrix.txt", "rt");
	if (file == NULL) return;
	char line[100];
	
	for (int i = 0; i < 5; i++)
	{
		if (fgets(line, 100, file) == NULL) break;
		for (int j = 0; j < 6; j++)
		{
			matrix[i][j] = calloc(4, sizeof(char));
		}
		sscanf(line, "%s %s %s %s %s %s", matrix[i][0], matrix[i][1], matrix[i][2], matrix[i][3], matrix[i][4], matrix[i][5]);
		for (int j = 0; j < 6; j++)
		{
			if (strcmp(matrix[i][j], "-") == 0) continue;
			char name[7];
			sprintf(name, "user%d", (i+1));
			AddACNode(fileLists[j], name, matrix[i][j]);
		}
	}
	fclose(file);
}
//gets the user's number from the username
int stringNum(const char * str)
{
	return str[4] - '0';
}
//deletes all permissions from an access control list (removes the node entirely from the ACL)
void DeletePermissions(ACList * list, const char * username)
{
	ACNode * temp = list->firstNode;
	if (strcmp(temp->username, username) == 0)
	{
		if (list->firstNode->next != NULL)
		{
			ACNode * temp2 = list->firstNode->next;
			free(list->firstNode);
			list->firstNode = temp2;
		}
		else
		{
			free(list->firstNode);
			list->firstNode = NULL;
		}
	}
	else
	{
		while (temp->next != NULL)
		{
			if (strcmp(temp->next->username, username) == 0)
			{
				if (temp->next->next != NULL)
				{
					ACNode * temp2 = temp->next->next;
					free(temp->next);
					temp->next = temp2;
				}
				else
				{
					free(temp->next);
					temp->next = NULL;
				}
				break;
			}
			temp = temp->next;
		}
	}
}
//adds permission to an access control list (creates a new node in the linked list)
void AddPermissions(ACList * list, const char * username, const char * permissions)
{
	int usernum = stringNum(username);
	ACNode * temp = list->firstNode;
	ACNode * newnode = CreateACNode(username, permissions);
	if (stringNum(temp->username) > usernum)
	{
		newnode->next = list->firstNode;
		list->firstNode = newnode;
	}
	else
	{
		while (temp != NULL)
		{
			if (temp->next == NULL || stringNum(temp->next->username) > usernum)
			{
				newnode->next = temp->next;
				temp->next = newnode;
				break;
			}
			temp = temp->next;
		}
	}
}
//edits permissions in the access control list (edits an existing node in the ACL)
void EditPermissions(ACList * list, const char * username, const char * permissions)
{
	ACNode * temp = list->firstNode;
	while (temp != NULL)
	{
		if (strcmp(temp->username, username) == 0)
		{
			strcpy(temp->permissions, permissions);
			break;
		}
		temp = temp->next;
	}
}
//writes the new matrix file after it has been edited.
void WriteMatrixFile(){
	FILE * matrixFile;
	if ((matrixFile = fopen("matrix.txt", "w")) == NULL)
	{
		return;
	}
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			fprintf(matrixFile, "%s ", matrix[i][j]);
		}
		fprintf(matrixFile, "\n");
	}
	fclose(matrixFile);
	return;
}
//edits the access matrix based on certain permissions, username and filename (involves updating the associated ACL and the access matrix)
void EditAccessMatrix(const char * username, const char * filename, const char * permissions)
{
	int filenum = stringNum(filename) - 1;
	int usernum = stringNum(username) - 1;
	char * prevPerm = matrix[usernum][filenum];
	if (strcmp(prevPerm, permissions) == 0)
	{
		printf("The permissions were already %s\n", permissions);
		return;
	}
	if (strcmp(permissions, "-") == 0)
	{
		//delete permissions
		DeletePermissions(fileLists[filenum], username);
		
	}
	else if (strcmp(prevPerm, "-") == 0)
	{
		//new permissions
		AddPermissions(fileLists[filenum], username, permissions);
	}
	else
	{
		//edit permissions
		EditPermissions(fileLists[filenum], username, permissions);
	}

	strcpy(matrix[stringNum(username) - 1][stringNum(filename) - 1], permissions);

	WriteMatrixFile();
}
//makes an access request of a certain user to file with certain permissions, which will be checked against the access control list
void AccessRequest(const char * username, const char * filename, const char * permissions)
{
	for (int i = 0; i < 6; i++)
	{
		ACList * list = fileLists[i];
		if (strcmp(list->filename, filename) == 0)
		{
			ACNode * node = list->firstNode;
			while (node != NULL)
			{
				if (strcmp(node->username, username) == 0)
				{
					int granted = 1;
					for (int j = 0; j < strlen(permissions); j++)
					{
						char p = permissions[j];
						if (p == 'r' || p == 'w' || p == 'x')
						{
							int found = 0;
							for (int k = 0; k < strlen(node->permissions); k++)
							{
								char pp = node->permissions[k];
								if (p == pp)
								{
									found = 1;
								}
							}
							if (found != 1)
							{
								granted = 0;
							}
						}
					}
					if (granted == 1)
					{
						printf("Your '%s' access request to the file '%s' is granted\n", permissions, filename);
					}
					else
					{
						printf("You only have '%s' access rights to the file '%s'\n", node->permissions, filename);
					}
					return;
				}
				node = node->next;
			}
			printf("The username did not have a permissions record for this file.\n");
			return;
		}
	}
	printf("The file was not found in the system.\n");

}