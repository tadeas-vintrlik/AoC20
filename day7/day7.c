#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LINE 200
#define MAX_BAG 30
#define MAX_SUB 10
#define START_SUB 14
#define MAX_VALID 600

typedef struct 
{
	char main[MAX_BAG];
	char sub[MAX_LINE];
} bag_t;

// Store line from index start until index end in main
void get_substr(char bag[MAX_LINE], const char line[MAX_LINE], int start, int end)
{
	for (int i = start; i < end; i++)
	{
		bag[i-start] = line[i];
		if (line[i] == '\n')
			bag[i - start] = '\0';
	}
	bag[end] = '\0';
}

bool get_bag(FILE *file, bag_t *bag)
{
	char line[MAX_LINE] = "";
	if (!fgets(line, MAX_LINE, file))
		return false;
	char *end_main = strstr(line, "bags");
	char main[MAX_BAG] = "";
	// -1 to get rid of the whitespace
	int end_main_i = end_main - line - 1;
	// Get bag colour
	get_substr(main, line, 0, end_main_i);
	// Get all of the sub bags
	int start_sub = end_main_i + START_SUB;
	get_substr(line, line, start_sub, strlen(line)+1);
	strcpy(bag->main, main);
	strcpy(bag->sub, line);
	return true;
}

bool contains_valid(bag_t bag, char valid[MAX_VALID][MAX_BAG], int amount)
{
	for (int i = 0; i < amount; i++)
		if (strstr(bag.sub, valid[i]))
			return true;
	return false;
}

bool in_valid(bag_t bag, char valid[MAX_VALID][MAX_BAG], int amount)
{
	for (int i = 0; i < amount; i++)
		if (strcmp(bag.main, valid[i]) == 0)
			return true;

	return false;
}

// Only direct check
bool contains_shiny_gold(bag_t bag, char valid[MAX_VALID][MAX_BAG], int amount)
{
	if (strstr(bag.sub, "shiny gold"))
		return true;
	if (contains_valid(bag, valid, amount))
		return true;
	return false;
}

int main(void)
{
	FILE *file = fopen("./day7.dat", "r");
	if (!file)
	{
		fprintf(stderr, "File could not be opened!\n");
		return EXIT_FAILURE;
	}

	char valid[MAX_VALID][MAX_BAG] = { "" };
	int amount = 0;
	int before_loop = 0;
	bag_t bag = { .main = "", . sub = "" };
	do
	{
		before_loop = amount;
		rewind(file);
		while (get_bag(file, &bag))
		{
			if (contains_shiny_gold(bag, valid, amount))
			{
				// It is enough to check just bags added since last loop
				if (!in_valid(bag, valid, amount))
					strcpy(valid[amount++], bag.main);
			}
		}
	} while(before_loop != amount);

	printf("Valid bags: %d\n", amount);
	fclose(file);
	return EXIT_SUCCESS;
}
