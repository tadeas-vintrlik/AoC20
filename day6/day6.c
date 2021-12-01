#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define LINE 100
#define GROUP 1000
#define QUESTIONS 26


int get_group(FILE *file, char ans_text[GROUP])
{
	strcpy(ans_text, "");
	int no_people = 0;
	char local[LINE];
	if (fgets(local, LINE, file) == NULL)
		return 0;

	// Skip empty lines
	while (strcmp(local, "\n") == 0)
		fgets(ans_text, LINE, file);

	// While the same group
	while (strcmp(local, "\n") != 0)
	{
		no_people++;
		// Get rid of newline
		if (local[strlen(local) - 1] == '\n')
			local[strlen(local) - 1] = '\0';

		strcat(ans_text, local);
		if (fgets(local, LINE, file) == NULL)
			break;
	}
	return no_people;
}

int get_no_ans(const char ans_text[GROUP], int no_people, int *yes_two)
{
	int ans[QUESTIONS] = { 0 };
	int yes = 0;
	for (int i = 0; ans_text[i] != '\0'; i++)
		ans[ans_text[i] - 'a']++;

	for (int i = 0; i < QUESTIONS; i++)
	{
		if (ans[i] > 0)
			yes++;
		if (ans[i] == no_people)
			*yes_two += 1;
	}
	return yes;
}

int main(void)
{
	FILE *file = fopen("./day6.dat", "r");
	if (!file)
	{
		fprintf(stderr, "Could not open file!\n");
		return EXIT_FAILURE;
	}

	char ans_text[GROUP] = "";
	int sum = 0;
	int sum_two = 0;
	int no_people = 0;
	while ((no_people = get_group(file, ans_text)) != 0)
	{
		int agree_two = 0;
		sum += get_no_ans(ans_text, no_people, &agree_two);
		sum_two += agree_two;
	}

	printf("Sum of positive answers: %d\n", sum);
	printf("Part two sum of positive answers: %d\n", sum_two);
	fclose(file);
	return EXIT_SUCCESS;
}
