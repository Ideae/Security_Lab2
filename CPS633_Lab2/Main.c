#include "Headers.h"
//the threshold that will be input by the user and used in later calculations
double threshold = 0.0;
//the main loop which will allow the user to pick from a menu, to either edit the matrix or authenticate and enter either the ACL program or RBAC program
int main()
{
	PopulateUserData();
	ReadAccessControlMatrix();

	while (1)
	{
		printf("Security Lab2: Enter a command:\n");
		printf("0. Exit program\n1. Login ACL\n2. Login RBAC\n3. Edit Access Control Matrix\n4. Print Access Control Lists\n5. Authentication Setup\n> ");
		int command = -1;
		scanf("%d", &command);
		if (command == 0) return 0;
		if (command == 1)
		{
			char * username = calloc(8, sizeof(char));
			if (mainLogin(username))
			{
				mainAuthorization(username);
			}
		}
		else if (command == 2)
		{
			char * username = calloc(8, sizeof(char));
			if (mainLogin(username))
			{
				mainRBAC(username);
			}
		}
		else if (command == 3)
		{
			mainEditMatrix();
		}
		else if (command == 4)
		{
			PrintACLists();
		}
		else if (command == 5)
		{
			mainAuthentication();
		}
	}
	return 0;
}
//the input loop which allows the user to attempt authentication with the OKAM system
int mainLogin(char * username)
{
	int verfNum = 0;
	printf("Please enter a threshold percentage value: ");
	scanf("%lf", &threshold);
	printf("Please login. username: ");
	scanf("%s", username);
	printf("Verification number (1-5): ");
	scanf("%d", &verfNum);
	int result = AttemptLogin(username[4] - '0', verfNum - 1, threshold);
	if (result == 1)
	{
		printf("Access Granted!\n");
		return 1;
	}
	else
	{
		printf("Access Failed!\n");
		return 0;
	}
	return 0;
}