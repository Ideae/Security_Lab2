#include "Headers.h"

int enrollData[5][500][2];
int verifyData[5][2500][2];

int mainAuthentication()
{
	//PopulateUserData();
	while(1)
	{
		int usernum = -1;
		double input = -1;
		printf("Please enter the user number and the threshold as a percentage: ");
		scanf("%d %lf", &usernum, &input);
		if (input == 0)
		{
			return 0;
		}
		//printf("Read in a %d\n", input);
		InitOKAM(usernum, input);
	}
	return 0;
}

void PopulateUserData()
{
	//read in data
	for (int usernum = 0; usernum < 5; usernum++)
	{
		char* filename = calloc(100, sizeof(char));
		sprintf(filename, "user_data//user%d.txt", (usernum + 1));
		FILE * file = fopen(filename, "rt");
		if (file == NULL) return;
		char line[100];
		char newLine = '\n';
		fgets(line, 100, file);
		int fly, dwell, other;
		for (int i = 0; i < 500; i++)
		{
			if (fgets(line, 100, file) == NULL) break;
			sscanf(line, "%d %d %d %d %d", &other, &other, &fly, &dwell, &other);
			enrollData[usernum][i][0] = fly;
			enrollData[usernum][i][1] = dwell;
		}
		for (int i = 0; i < 2500; i++)
		{
			if (fgets(line, 100, file) == NULL) break;
			sscanf(line, "%d %d %d %d %d", &other, &other, &fly, &dwell, &other);
			verifyData[usernum][i][0] = fly;
			verifyData[usernum][i][1] = dwell;
		}
		fclose(file);
	}
}

void InitOKAM(int userNumber, double threshold)
{
	//userNumber++; //?
	userNumber--;

	//read in monitored data (verification phase)
	int falseRejects = 0;
	for (int verifySet = 0; verifySet < 5; verifySet++)
	{
		double digraphSum = 0, monographSum = 0;
		for (int i = 0; i < 500; i++)
		{
			int index = 500 * verifySet + i;
			int fly = verifyData[userNumber][index][0];
			int dwell = verifyData[userNumber][index][1];
			if (fly != 0) digraphSum += (abs(fly - enrollData[userNumber][i][0])) / (double)fly;
			if (dwell != 0) monographSum += (abs(dwell - enrollData[userNumber][i][1])) / (double)dwell;
		}
		double D = ((digraphSum / 499.0) + (monographSum / 500.0)) * 50.0;
		if (D >= threshold)
		{
			falseRejects++;
		}
		printf("D = %lf\n", D);
	}
	double FRR = falseRejects / 5.0;

	int falseAccepts = 0;
	for (int otherUser = 0; otherUser < 5; otherUser++)
	{
		if (otherUser == userNumber) continue;
		for (int verfySet = 0; verfySet < 5; verfySet++)
		{
			double digraphSum = 0, monographSum = 0;
			for (int i = 0; i < 500; i++)
			{
				int index = 500 * verfySet + i;
				int fly = verifyData[otherUser][index][0];
				int dwell = verifyData[otherUser][index][1];
				if (fly != 0) digraphSum += (abs(fly - enrollData[userNumber][i][0])) / (double)fly;
				if (dwell != 0) monographSum += (abs(dwell - enrollData[userNumber][i][1])) / (double)dwell;
			}
			double D = ((digraphSum / 499.0) + (monographSum / 500.0)) * 50.0;
			if (D <= threshold)
			{
				falseAccepts++;
			}
			printf("D = %lf\n", D);
		}
	}

	double FAR = falseAccepts / (5.0 * 4.0);
	printf("FRR : %lf  FAR : %lf\n", FRR, FAR);
	if (FAR == FRR)
	{
		printf("An Equal Error Rate has been achieved.\n");
	}
	else
	{
		printf("An Equal Error Rate has not been achieved.\n");
	}
}

int AttemptLogin(int userNumber, int verifySet, double threshold)
{
	double digraphSum = 0, monographSum = 0;
	for (int i = 0; i < 500; i++)
	{
		int index = 500 * verifySet + i;
		int fly = verifyData[userNumber][index][0];
		int dwell = verifyData[userNumber][index][1];
		if (fly != 0) digraphSum += (abs(fly - enrollData[userNumber][i][0])) / (double)fly;
		if (dwell != 0) monographSum += (abs(dwell - enrollData[userNumber][i][1])) / (double)dwell;
	}
	double D = ((digraphSum / 499.0) + (monographSum / 500.0)) * 50.0;
	if (D <= threshold)
	{
		return 1;
	}
	return 0;
}