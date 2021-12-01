#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 500
#define ONE_LINE 100
#define KEY_LEN 4
#define MAX_KEY 7
#define VALID_ID_SUM 28 // Sum of all indexes of VALID + 1
// 1 + 2 + 3 + 4 + 5 + 6 + 7
const char VALID[MAX_KEY][KEY_LEN] = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
enum KEYS { BYR, IYR, EYR, HGT, HCL, ECL, PID_E };
const char EYE_COLOR[MAX_KEY][KEY_LEN] = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };

void replace_newlines(char passport[MAX])
{
	for (int i = 0; passport[i] != '\0'; i++)
		if (passport[i] == '\n')
			passport[i] = ' ';
}

bool load_one_passport(FILE *file, char passport[MAX])
{
	strcpy(passport, "");
	char temp[ONE_LINE] = "";
	if (fgets(temp, ONE_LINE, file) == NULL)
		return false;
	// Skip empty lines
	while (strcmp(temp, "\n") == 0)
			fgets(temp, ONE_LINE, file);

	while (strcmp(temp, "\n") != 0)
	{
		strcat(passport, temp);
		if (fgets(temp, ONE_LINE, file) == NULL)
			break;
	}
	replace_newlines(passport);
	return true;
}

int get_key_id(char *token)
{
	for (int i = 0; i < MAX_KEY; i++)
		if (strcmp(VALID[i], token) == 0)
			return i + 1;
	return 0;
}

bool valid_hcl(char value[ONE_LINE])
{
	if (value[0] != '#')
		return false;
	if (strlen(value) != 7)
		return false;
	for (int i = 1; value[i] != '\0'; i++)
	{
		if (value[i] >= '0' && value[i] <= '9')
			continue;
		else if (value[i] >= 'a' && value[i] <= 'f')
			continue;
		else
			return false;
	}
	return true;
}

bool valid_ecl(char value[ONE_LINE])
{
	for (int i = 0; i < MAX_KEY; i++)
		if (strcmp(EYE_COLOR[i], value) == 0)
			return true;
	return false;
}

bool valid_pid(char value[ONE_LINE])
{
	if (strlen(value) != 9)
		return false;
	for (int i = 0; i < 9; i++)
		if (value[i] < '0' || value[i] > '9')
			return false;
	return true;
}

bool check_field(int key_id, char value[ONE_LINE])
{
	char *endptr = NULL;
	int number = strtol(value, &endptr, 10);
	switch (key_id)
	{
		case BYR:
			return number >= 1920 && number <= 2002 && strlen(endptr) == 0;
			break;
		case IYR:
			return number >= 2010 && number <= 2020 && strlen(endptr) == 0;
			break;
		case EYR:
			return number >= 2020 && number <= 2030 && strlen(endptr) == 0;
			break;
		case HGT:
			if (strcmp(endptr, "cm") == 0)
				return number >= 150 && number <= 193;
			if (strcmp(endptr, "in") == 0)
				return number >= 59 && number <= 76;
			else
				return false;
			break;
		case HCL:
			return valid_hcl(value);
			break;
		case ECL:
			return valid_ecl(value);
			break;
		case PID_E:
			return valid_pid(value);
			break;
	}
	return true;
}

bool valid_passport(const char passport[MAX], int *valid_values)
{
	char local[MAX];
	strcpy(local, passport);
	char *token = strtok(local, " ");
	char key[KEY_LEN];
	int key_id_sum = 0;
	int current_id = 0;
	bool check_values = true;
	while (token)
	{
		// Get key
		memcpy(key, token, KEY_LEN - 1);
		// Get value
		char value[ONE_LINE] = "";
		for (int i = KEY_LEN; token[i] != '\0'; i++)
			value[i - KEY_LEN] = token[i];
		current_id = get_key_id(key);

		if (current_id > 0 && !check_field(current_id - 1, value))
			check_values = false;

		key_id_sum += current_id;
		token = strtok(NULL, " ");
	}
	if (key_id_sum == VALID_ID_SUM && check_values)
		*valid_values += 1;
	return key_id_sum == VALID_ID_SUM;
}

int main(void)
{
	FILE *file = fopen("./day4.dat", "r");
	if (!file)
	{
		fprintf(stderr, "Could not open the file\n");
		return EXIT_FAILURE;
	}

	char passport[MAX];
	int valid_one = 0;
	int valid_two = 0;
	int total = 0;
	while (load_one_passport(file, passport))
	{
		total++;
		if(valid_passport(passport, &valid_two))
			valid_one++;   	
	}

	printf("Valid passports: %d/%d\n", valid_one, total);
	printf("Valid passports and values: %d/%d\n", valid_two, total);
	fclose(file);
	return EXIT_SUCCESS;
}
