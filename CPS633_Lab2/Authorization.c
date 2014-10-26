#include "Headers.h"
ACList* fileLists[6];
char* matrix[5][6];

int main()
{
	CreateAccessControlMatrix();
	PrintACLists();
	char * filename = calloc(8, sizeof(char));
	char * username = calloc(8, sizeof(char));
	char * permissions = calloc(8, sizeof(char));
	while (1)
	{
		printf("Please enter access request as follows: username filename permissions\n");
		scanf("%s %s %s", username, filename, permissions);
		AccessRequest(username, filename, permissions);
	}
	return 0;
}

void PrintACLists()
{
	for (int i = 0; i < 6; i++)
	{
		PrintACL(fileLists[i]);
	}
}

void CreateAccessControlMatrix()
{
	for (int i = 0; i < 6; i++)
	{
		char name[7];
		sprintf(name, "file%d", (i+1));
		fileLists[i] = CreateACList(name);// +(i + 1));
	}


	FILE * file = fopen("matrix.txt", "rt");
	if (file == NULL) return;
	char line[100];
	char newLine = '\n';
	
	for (int i = 0; i < 5; i++)
	{
		if (fgets(line, 100, file) == NULL) break;
		for (int j = 0; j < 6; j++)
		{
			matrix[i][j] = calloc(4, sizeof(char));
		}
		sscanf(line, "%s %s %s %s %s %s", matrix[i][0], matrix[i][1], matrix[i][2], matrix[i][3], matrix[i][4], matrix[i][5]);
		//printf("%s %s %s %s %s %s\n", matrix[i][0], matrix[i][1], matrix[i][2], matrix[i][3], matrix[i][4], matrix[i][5]);
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

void TestACList()
{
	ACList * list = CreateACList("file1.txt");
	AddACNode(list, "zharris", "rwx");
	AddACNode(list, "dcamaren", "r-x");
	PrintACL(list);
}