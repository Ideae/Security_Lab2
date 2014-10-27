#include "Headers.h"
#define ROLECOUNT 3
#define USERCOUNT 5
#define FILECOUNT 6
//the string name representations of each role, indexed by role numbers
char * roleNames[ROLECOUNT];
//the table containing the roles each user has
int userRoles[USERCOUNT][ROLECOUNT];
//the table containing the permissions of each role for every file in the system
char * rolePermissionsFiles[ROLECOUNT][FILECOUNT];
//the table containing the role assignment permissions for every role
int assignPermissionsRoles[ROLECOUNT][ROLECOUNT];
//a flag to check whether the RBAC system was previously initialized
int initPreviously = 0;
//the input loop that lets the user interact with the RBAC authorization system
int mainRBAC(char * username)
{
	if (!initPreviously++)
		InitRBAC();
	char * filename = calloc(8, sizeof(char));
	char * permissions = calloc(8, sizeof(char));
	char * newrole = calloc(10, sizeof(char));
	char * otheruser = calloc(8, sizeof(char));
	while (1)
	{
		printf("Please select from the following:\n0. Exit\n1. Access request of file.\n2. Set a user's role\n");
		int input = -1;
		scanf("%d", &input);
		if (input == 0)
		{
			return 0;
		}
		else if (input == 1)
		{
			printf("Please enter access request as follows (0 0 to exit):\nfilename permissions\n");
			scanf("%s %s", filename, permissions);
			if (filename[0] == '0' && permissions[0] == '0')
			{
				return 0;
			}
			RequestRBAC(username, filename, permissions);
		}
		else if (input == 2)
		{
			printf("Please change a user's role as follows (0 0 to exit):\nusername newrole\n");
			scanf("%s %s", otheruser, newrole);
			if (otheruser[0] == '0' && newrole[0] == '0')
			{
				return 0;
			}
			SetRole(otheruser, newrole);
		}
	}
	return 0;
}
//allows a user to make an access control request through the RBAC system, taking into account their roles for the filename provided
void RequestRBAC(const char * username, const char * filename, const char * permissions)
{
	int usernum = username[4] - '0' - 1;
	int filenum = filename[4] - '0' - 1;
	int perms[3];
	for (int i = 0; i < ROLECOUNT; i++)
	{
		if (userRoles[usernum][i] == 1)
		{
			char * p = rolePermissionsFiles[i][filenum];
			for (int j = 0; j < strlen(p); j++)
			{
				if (p[j] == 'r') perms[0] = 1;
				else if (p[j] == 'w') perms[1] = 1;
				else if (p[j] == 'x') perms[2] = 1;
			}
		}
	}
	int fail = 0;
	for (int i = 0; i < strlen(permissions); i++)
	{
		if (permissions[i] == 'r' && perms[0] != 1) fail = 1;
		else if (permissions[i] == 'w' && perms[1] != 1) fail = 1;
		else if (permissions[i] == 'x' && perms[2] != 1) fail = 1;
	}
	if (fail == 0)
	{
		printf("Your '%s' access request to the file '%s' is granted\n", permissions, filename);
	}
	else
	{
		//printf("You only have '%s' access rights to the file '%s'\n", node->permissions, filename);
		printf("You only have ");
		for (int i = 0; i < ROLECOUNT; i++)
		{
			if (userRoles[usernum][i] == 1)
			{
				char * p = rolePermissionsFiles[i][filenum];
				printf("%s, ", p);
			}
		}
		printf(" access rights to the file '%s'\n", filename);
	}
}
//allows a user logged into the system to change the role of a user, taking the RBAC rules into account (only one manager, etc)
void SetRole(char * username, char * newrole)
{
	int rolenum = -1;
	for (int i = 0; i < ROLECOUNT; i++)
	{
		if (strcmp(roleNames[i], newrole) == 0)
		{
			rolenum = i;
		}
	}
	if (rolenum == -1)
	{
		printf("Error: New role did not exist.\n");
		return;
	}
	int usernum = username[4] - '0';
	if (userRoles[usernum][rolenum] == 1)
	{
		printf("User already had the role %s.\n", newrole);
		return;
	}
	if (rolenum == 0)
	{
		for (int i = 0; i < USERCOUNT; i++)
		{
			if (userRoles[i][0] == 1)
			{
				printf("Error, a manager already exists.\n");
				return;
			}
		}
	}
	userRoles[usernum][rolenum] = 1;
	printf("%s has been given the new role %s\n", username, newrole);
}
//initializes the RBAC system and populates all associated tables with information
void InitRBAC()
{
	roleNames[0] = calloc(10, sizeof(char));
	strcpy(roleNames[0], "Manager");
	roleNames[1] = calloc(10, sizeof(char));
	strcpy(roleNames[1], "Technical");
	roleNames[2] = calloc(10, sizeof(char));
	strcpy(roleNames[2], "Sales");

	userRoles[0][0] = 1;
	userRoles[1][1] = userRoles[2][1] = 1;
	userRoles[3][2] = userRoles[4][2] = 1;

	assignPermissionsRoles[0][1] = assignPermissionsRoles[0][2] = 1;
	assignPermissionsRoles[1][2] = 1;
	
	FILE * file = fopen("matrix.txt", "rt");
	if (file == NULL) return;
	char line[100];

	for (int i = 0; i < 3; i++)
	{
		if (fgets(line, 100, file) == NULL) break;
		for (int j = 0; j < 6; j++)
		{
			rolePermissionsFiles[i][j] = calloc(4, sizeof(char));
		}
		sscanf(line, "%s %s %s %s %s %s", rolePermissionsFiles[i][0], rolePermissionsFiles[i][1], rolePermissionsFiles[i][2], rolePermissionsFiles[i][3], rolePermissionsFiles[i][4], rolePermissionsFiles[i][5]);
	}
	fclose(file);
}