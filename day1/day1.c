#include <stdio.h>
#include <stdlib.h>

#define MAX 1000

// return number of found entires + 1
int fill(FILE *file, int arr[MAX])
{
	int count = 0;
	while ((fscanf(file, "%d", &arr[count++]) != EOF));
	return count;
}

int main(void)
{
	int arr[MAX];
	FILE *file = fopen("./day1.dat", "r");
	if (!file)
	{
		fprintf(stderr, "Failed to open file!\n");
		return EXIT_FAILURE;
	}
	int len = fill(file, arr);

	for (int i = 0; i < len; i++)
		for (int j = i+1; j < len; j++)
			if (arr[i] + arr[j] == 2020)
				printf("Sum of two result: %d * %d = %d\n",
						arr[i], arr[j], arr[i] * arr[j]);

	for (int i = 0; i < len; i++)
		for (int j = i+1; j < len; j++)
			for (int k = j+1; k < len; k++)
				if (arr[i] + arr[j] + arr[k] == 2020)
					printf("Sum of three result: %d * %d * %d = %d\n",
							arr[i], arr[j], arr[k],
							arr[i] * arr[j] * arr[k]);

	fclose(file);
	return EXIT_SUCCESS;
}
