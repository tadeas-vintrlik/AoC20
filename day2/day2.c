#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LINES 2000
#define MAX_LENGTH 30

int get_matching_passwords(FILE *file, int *new_job)
{
	char input[LINES][MAX_LENGTH];
	char current[MAX_LENGTH];

	int amount = 0;

	// Load the entire file into the input variable
	while (fgets(current, MAX_LENGTH, file))
		strcpy(input[amount++], current);

	int matching_passwd = 0;
	// Loop through all lines
	for (int i = 0; i < amount; i++)
	{
		char *endptr1 = NULL;
		int low = strtol(input[i], &endptr1, 10);
		int high = -strtol(endptr1, NULL, 10);
		char *colon = strchr(input[i], ':');
		if (colon == NULL)
			continue;
		int colon_index = colon - input[i];
		// Find character to check
		char to_check = input[i][colon_index - 1];

		int count = 0;
		// Find number of occurrences of to_check
		for (int j = colon_index + 2; j < MAX_LENGTH ; j++)
			if(input[i][j] == to_check)
				count++;

		// Return if matching the criteria
		if (count >= low && count <= high)
			matching_passwd++;

		// Check for the second criteria
		bool first_match = input[i][colon_index + low + 1] == to_check;
		bool second_match = input[i][colon_index + high + 1] == to_check;

		if (first_match && second_match)
			continue;

		if (first_match)
			*new_job+=1;

		if (second_match)
			*new_job+=1;
	}
	return matching_passwd;
}

int main(void)
{
	FILE *file = fopen("day2.dat", "r");
	if (!file)
	{
		fprintf(stderr, "File could not be opened!\n");
		return EXIT_FAILURE;
	}

	int new_job = 0;
	printf("Matching passwords old job: %d\n",
			get_matching_passwords(file, &new_job));
	printf("Matching passwords new job: %d\n", new_job);

	fclose(file);
	return EXIT_SUCCESS;
}
