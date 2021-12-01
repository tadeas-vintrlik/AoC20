#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 2000
#define LINE 50
#define PREAMBLE 25

typedef struct
{
	int values[MAX];
	int amount;
	int preamble_start;
} numbers_t;

void load_numbers(numbers_t *numbers, FILE *file)
{
	char line[LINE];
	while (fgets(line, LINE, file))
		numbers->values[numbers->amount++] = strtol(line, NULL, 10);
}

bool is_sum_preamble(numbers_t *numbers, int number)
{
	int start = numbers->preamble_start++;
	for (int i = start; i < start + PREAMBLE; i++)
		for (int j = i + 1; j < start + PREAMBLE; j++)
			if (numbers->values[i] + numbers->values[j] == number)
				return true;
	return false;
}

/**
 * Find a sequence of numbers adds up to invalid
 * @param numbers - list of all of the numbers
 * @param invalid - number to try and match
 * @param start - where to start the search
 * @param last - where the high number of the sum will be stored
 *
 */
bool adds_to_invalid(numbers_t numbers, int invalid, int start, int *min, int *max)
{
	int sum = 0;
	int local_min = numbers.values[start];
	int local_max = numbers.values[start];
	for (int i = start; i < numbers.amount; i++)
	{
		int current = numbers.values[i];
		if (current > local_max)
			local_max = current;
		if (current < local_min)
			local_min = current;
		sum += current;
		if (sum > invalid)
			return false; // If overblown it cannot be invalid
		if (sum == invalid)
		{
			*min = local_min;
			*max = local_max;
			return true;
		}
	}
	return  false;
}

int main(void)
{
	FILE *file = fopen("day9.dat", "r");
	if (!file)
	{
		fprintf(stderr, "Could not open the file!\n");
		return EXIT_FAILURE;
	}
	numbers_t numbers = { .amount = 0, .preamble_start = 0};
	int invalid = 0;
	load_numbers(&numbers, file);
	fclose(file);
	for (int i = PREAMBLE; i < numbers.amount; i++)
	{
		if (!is_sum_preamble(&numbers, numbers.values[i]))
		{
			invalid = numbers.values[i];
			break;
		}
	}

	printf("%d Was not a sum of preamble!\n", invalid);
	int min = 0;
	int max = 0;
	int result = 0; // sum of min and max
	for (int i = 0; i < numbers.amount; i++)
	{
		if(adds_to_invalid(numbers, invalid, i, &min, &max))
		{
			result = min + max;
			break;
		}
	}
	
	printf("%d was the encryption weakness!\n", result);
	return EXIT_FAILURE;
}
