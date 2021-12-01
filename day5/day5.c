#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_TEXT 12
#define MAX 1000
#define SEATS 1024
#define DEFAULT_RANGE_ROW 128
#define DEFAULT_RANGE_COL 8
#define ROW_INDEX_MAX 7
#define COL_INDEX_MAX 10

bool get_partition(FILE *file, char text[MAX_TEXT])
{
	if (fgets(text, MAX_TEXT, file) == NULL)
		return false;
	return true;
}

int get_row(const char text[MAX_TEXT]) { int range_size = DEFAULT_RANGE_ROW;
	int low = 0, high = range_size - 1;
	for (int i = 0; i < ROW_INDEX_MAX; i++)
	{
		range_size /= 2;
		if (text[i] == 'F')
			high -= range_size;
		else
			low += range_size;
	}
	return low;
}

int get_col(const char text[MAX_TEXT])
{
	int range_size = DEFAULT_RANGE_COL;
	int low = 0, high = range_size - 1;
	for (int i = ROW_INDEX_MAX; i < COL_INDEX_MAX; i++)
	{
		range_size /= 2;
		if (text[i] == 'L')
			high -= range_size;
		else
			low += range_size;
	}
	return low;
}

int main(void)
{
	FILE *file = fopen("./day5.dat", "r");
	if (!file)
	{
		fprintf(stderr, "File could not be opened!\n");
		return EXIT_FAILURE;
	}

	char text[MAX_TEXT] = "";
	int max = 0;
	bool sieve[SEATS] = { false };
	while (get_partition(file, text))
	{
		int row = get_row(text);
		int col = get_col(text);
		int result = row * 8 + col;
		sieve[result] = true;
		if (result > max)
			max = result;
	}

	printf("Maximum Seat ID: %d\n", max);
	for (int i = 1; i < max; i++)
		if (!sieve[i]&& sieve[i-1] && sieve[i+1])
			printf("My Seat ID is: %d\n", i);
	fclose(file);
	return EXIT_SUCCESS;
}

