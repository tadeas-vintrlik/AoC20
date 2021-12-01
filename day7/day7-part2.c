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

typedef struct
{
	char name[MAX_BAG];
	int contains;	// Amount of bags contained inside including self
} bag_info_t;

typedef struct
{
	bag_info_t bags[MAX_VALID];
	int amount;
} valid_array_t;

// Store line from index start until index end in main
void get_substr(char bag[MAX_LINE], const char line[MAX_LINE], int start, int end)
{
	for (int i = start; i < end; i++)
	{
		bag[i-start] = line[i];
		if (line[i] == '\n')
			bag[i - start] = '\0';
	}
	bag[end-1] = '\0';
}

bool get_bag(FILE *file, bag_t *bag)
{
	char line[MAX_LINE] = "";
	if (!fgets(line, MAX_LINE, file))
		return false;
	char *end_main = strstr(line, "bags");
	char main[MAX_BAG] = "";
	int end_main_i = end_main - line;
	// Get bag colour
	get_substr(main, line, 0, end_main_i);
	// Get all of the sub bags
	int start_sub = end_main_i + START_SUB - 1;
	get_substr(line, line, start_sub, strlen(line));
	strcpy(bag->main, main);
	strcpy(bag->sub, line);
	return true;
}

int in_valid(char name[MAX_BAG], valid_array_t valid)
{
	for (int i = 0; i < valid.amount; i++)
		if (strcmp(name, valid.bags[i].name) == 0)
			return i;

	return -1;
}

// Return true if can determine, false if not
bool get_bag_info(bag_t bag, bag_info_t *bag_info, valid_array_t valid)
{
	strcpy(bag_info->name, bag.main);
	bag_info->contains = 1; // Self by default
	char *token = strtok(bag.sub, ",");
	int sum = 0;
	int index = 0;
	while (token)
	{
		char *endptr = NULL;
		int amount = strtol(token, &endptr, 10);
		char *colour_end = strstr(endptr, "bag");
		// Bag contained no other bags
		if (amount == 0)
			return true;
		else
		{
			// Get sub bag name and try to find in the valid array
			get_substr(endptr, endptr, 1, colour_end - endptr - 1);
			if ((index = in_valid(endptr, valid)) >= 0)
				sum += amount * valid.bags[index].contains;
			else
				return false;
			token = strtok(NULL, ",");
		}
	}
	bag_info->contains += sum;
	return true;
}

int main(void)
{
	FILE *file = fopen("./day7.dat", "r");
	if (!file)
	{
		fprintf(stderr, "File could not be opened!\n");
		return EXIT_FAILURE;
	}

	valid_array_t valid = {.amount = 0 };
	int before_loop = 0;
	do
	{
		before_loop = valid.amount;
		rewind(file);
		bag_t bag = { .main = "", . sub = "" };
		while (get_bag(file, &bag))
		{
			bag_info_t bag_info;
			if (get_bag_info(bag, &bag_info, valid))
				if (in_valid(bag_info.name, valid) == -1)
				{
					if (strcmp(bag_info.name, "shiny gold") == 0)
					{
						// -1 since we do not care about the bag itself
						printf("Contains bags: %d\n", bag_info.contains - 1);
						fclose(file);
						return EXIT_SUCCESS;
					}
					else
						valid.bags[valid.amount++] = bag_info;
				}
		}
	} while(before_loop != valid.amount);
	// Should not get here
	fprintf(stderr, "Could not get to shiny gold bag, was the input correct?\n");
	fclose(file);
	return EXIT_FAILURE;
}
