#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 100
#define LEAVE_1 4
#define LEAVE_2 5

typedef struct
{
	char data[MAX*MAX];
	int rows;
	int cols;
} matrix_t;

void set_cell(matrix_t *matrix, unsigned row, unsigned col, char new)
{
	matrix->data[row * matrix->cols + col] = new;
}

char get_cell(const matrix_t *matrix, unsigned row, unsigned col)
{
	return matrix->data[row * matrix->cols + col];
}

matrix_t load_seats(FILE *file)
{
	char line[MAX];
	fgets(line, MAX, file);
	int cols = strlen(line) - 1;	// Without newline symbol
	matrix_t new = { .data=".", .rows = 0, .cols = cols };
	do
	{
		for (int col = 0; line[col] != '\n'; col++)
			set_cell(&new, new.rows, col, line[col]);
		new.rows++;
	} while(fgets(line, MAX, file));
	return new;
}

// Return if valid coordinate, since segmentation faults are not nice
bool valid_crd(const matrix_t *matrix, int row, int col)
{
	return row >= 0 && row < matrix->rows && col >= 0 && col < matrix->cols;
}

// Return if occupied seat can be seen in the given direction
// x is gain in the x axis (+1, 0, -1), y is the same for y axis
bool seat_direction(const matrix_t *matrix, int row, int col, int x, int y)
{
	if (x == 0 && y == 0)
		return false;	// This would result in an endless loop
	row += x;
	col += y;
	while (valid_crd(matrix, row, col))
	{
		char tile = get_cell(matrix, row, col);
		switch (tile)
		{
			case '#':
				return true;
			case 'L':
				return false;
		}
		row += x;
		col += y;
	}
	return false;
}

unsigned surrounds_part2(const matrix_t *matrix, int row, int col)
{
	int occupied = 0;
	for (int x = -1; x < 2; x++)
		for (int y = -1; y < 2; y++)
			if (seat_direction(matrix, row, col, x, y))
				occupied++;
	return occupied;
}

unsigned surrounds_part1(const matrix_t *matrix, int row, int col)
{
	int occupied = 0;
	if (get_cell(matrix, row, col) == '#')
		occupied = -1;	// If the seat itself was occupied it will be included
	for (int i = row - 1; i != row + 2; i++)
		for (int j = col -1; j != col + 2; j++)
			if (valid_crd(matrix, i, j) && get_cell(matrix, i, j) == '#')
				occupied++;
	return occupied;
}

bool simulate_round(matrix_t *matrix, unsigned needed_to_leave,
		unsigned (*surround) (const matrix_t*, int, int), int *occupied)
{
	matrix_t local = *matrix;
	*occupied = 0;
	bool changes = false;
	for (int i = 0; i < matrix->rows; i++)
	{
		for (int j = 0; j < matrix->rows; j++)
		{
			char seat = get_cell(&local, i, j);
			switch (seat)
			{
				case 'L':
					if (surround(&local, i, j) == 0)
					{
						set_cell(matrix, i, j, '#');
						changes = true;
					}
					break;
				case '#':
					if (surround(&local, i, j) >= needed_to_leave)
					{
						set_cell(matrix, i, j, 'L');
						changes = true;
					}
					break;
			} if (get_cell(&local, i , j) == '#')
				*occupied += 1;
		}
	}
	return changes;
}

int main(void)
{
	FILE *file = fopen("day11.dat", "r");

	if (!file)
	{
		fprintf(stderr, "File could not be opened!\n");
		return EXIT_FAILURE;
	}

	matrix_t load = load_seats(file);
	fclose(file);

	matrix_t matrix = load;
	int occupied = 0;
	while(simulate_round(&matrix, LEAVE_1, surrounds_part1, &occupied));
	printf("Part 1 solution: %d\n", occupied);

	matrix = load;
	occupied = 0;
	while(simulate_round(&matrix, LEAVE_2, surrounds_part2, &occupied));
	printf("Part 2 solution: %d\n", occupied);

	return EXIT_SUCCESS;
}
