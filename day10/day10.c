#include <stdio.h>
#include <stdlib.h>

#define MAX 100
#define LINE 50
#define DIFF 3	// Maximum difference between joltages to work

typedef struct
{
	int numbers[MAX];
	int amount;
} vector_t;

void load_numbers(FILE *file, vector_t *vector)
{
	char line[LINE];
	vector->numbers[vector->amount++] = 0;
	while(fgets(line, LINE, file))
		vector->numbers[vector->amount++] = strtol(line, NULL, 10);
	rewind(file);
}

int get_next_joltage(const vector_t vector, int joltage, int *diff)
{
	int min_index = -1;
	int min = DIFF + 1;
	for (int i = 0; i < vector.amount; i++)
	{
		int diff = vector.numbers[i] - joltage;
		// If a compatible minimal adapter
		if (diff > 0 && diff <= DIFF && diff < min)
		{
			min = diff;
			min_index = i;
		}
	}
	*diff = min;
	return vector.numbers[min_index];
}

int get_max(const vector_t vector)
{
	int max = vector.numbers[0];
	for (int i = 1; i < vector.amount; i++)
		if (vector.numbers[i] > max)
			max = vector.numbers[i];
	return max;
}

void join_adapters(const vector_t vector, int diffs[DIFF])
{
	int joltage = 0; // We start with 0 from the outlet
	int max = get_max(vector);
	while (joltage != max)
	{
		int diff = 0;
		joltage = get_next_joltage(vector, joltage, &diff);
		diffs[diff-1]++;
	}
	diffs[DIFF-1]++;	// Built-in has diff of 3
}

int find_min(vector_t vector, int start)
{
	int index_min = 0;
	int min = 0;
	for (int i = start; i < vector.amount; i++)
	{
		int current = vector.numbers[i];
		if (i == start || current < min)
		{
			min = current;
			index_min = i;
		}
	}
	return index_min;
}

void selection_sort(vector_t *vector)
{
	int sorted = 0;
	while (sorted != vector->amount - 1)
	{
		int index_min = find_min(*vector, sorted);
		int temp = vector->numbers[index_min];
		vector->numbers[index_min] = vector->numbers[sorted];
		vector->numbers[sorted++] = temp;
	}
}

// Return number of arrangements
long long int get_arrangements(vector_t vector)
{
	long long int jumps[MAX] = { 0 };
	jumps[0] = 1;	// Just one way to jump to zero
	int i = 1;
	for (; i < vector.amount; i++)
	{
		int current = vector.numbers[i];
		// printf("%d\n", current);
		for (int j = 1; j < DIFF + 1; j++)
		{
			if (i-j < 0)
				break;
			// If reachable by previous adapter
			if (vector.numbers[i-j] >= current - DIFF)
			{
				if (jumps[i-j] < 0)
					printf("negative index: %d\n", i-j);
				jumps[i] += jumps[i-j];
			}
			else
				break;
		}
	}
	return jumps[i-1];
}

int main(void)
{
	FILE *file = fopen("day10.dat", "r");
	if (!file)
	{
		fprintf(stderr, "File could not be opened!\n");
		return EXIT_FAILURE;
	}
	vector_t vector = { .amount = 0 };
	load_numbers(file, &vector);
	int diffs[DIFF] = { 0 }; // Field to store amount of differences of joltages

	join_adapters(vector, diffs);

	printf("%d difference of 1 jolt, %d differences of 2 jolts, \
%d differences of 3 jolts.\n", diffs[0], diffs[1], diffs[2]);
	printf("Part one result: %d.\n", diffs[0] * diffs[2]);

	selection_sort(&vector);
	long long int arr = get_arrangements(vector);
	printf("Part two result: %lld\n", arr);
	fclose(file);
	return EXIT_SUCCESS;
}
