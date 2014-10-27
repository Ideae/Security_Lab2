#include "Headers.h"

double threshold = 0.0;

int main()//Commands()
{
	PopulateUserData();
	//TestACList();
	ReadAccessControlMatrix();
	//PrintACLists();

	while (1)
	{
		printf("Security Lab2: Enter a command:\n");
		printf("0. Exit program\n1. Login\n2. Authorization\n3. Edit Access Control Matrix\n4. Print Access Control Lists\n5. Authentication Setup\n> ");
		int command = -1;
		scanf("%d", &command);
		if (command == 0) return 0;
		if (command == 1)
		{
			mainLogin();
		}
		else if (command == 2)
		{
			mainAuthorization();
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

int mainLogin()
{
	//PopulateUserData();
	//ReadAccessControlMatrix();
	char * username = calloc(8, sizeof(char));
	while (1)
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
		}
		else
		{
			printf("Access Failed!\n");
		}
		//InitOKAM(username[4] - '0', threshold);
	}
	int num = 0;
	scanf("%d", &num);
	return 0;
}