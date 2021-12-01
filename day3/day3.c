#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 33

int get_no_trees(FILE *file, int right, int down)
{
	rewind(file);
	char line[MAX];
	int index = 0, trees = 0;
	fgets(line, MAX, file); // Skip first line
	int length = strlen(line) - 1; // Without newline
	while (fgets(line, MAX, file) != NULL)
	{
		int local_down = down;
		while (--local_down)
			fgets(line, MAX, file);
		index += right;
		index %= length;
		if (line[index] == '#')
			trees++;
	}
	return trees;
}

int main(void)
{
	FILE *file = fopen("./day3.dat", "r");

	if (!file)
	{
		fprintf(stderr, "Could not open the file\n");
		return EXIT_FAILURE;
	}

	printf("Tree encountered first slope: %d\n", get_no_trees(file, 3, 1));
	int trees_all_slopes = 1;
	trees_all_slopes *= get_no_trees(file, 1, 1);
	trees_all_slopes *= get_no_trees(file, 3, 1);
	trees_all_slopes *= get_no_trees(file, 5, 1);
	trees_all_slopes *= get_no_trees(file, 7, 1);
	trees_all_slopes *= get_no_trees(file, 1, 2);
	printf("Tree encountered all slopes: %d\n", trees_all_slopes);

	fclose(file);
	return EXIT_SUCCESS;
}
