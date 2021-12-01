#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 200
#define START 100000000000000

int main(void)
{
	FILE *file = fopen("day13.dat" , "r");
	if (!file)
	{
		fprintf(stderr, "File could not be opened!\n");
		return EXIT_FAILURE;
	}

	char depart[MAX];
	fgets(depart, MAX, file);
	unsigned time = strtoul(depart, NULL, 10);
	fgets(depart, MAX, file);
	unsigned buses[MAX] = { 0 };
	unsigned amount = 0;
	char *token;
	token = strtok(depart, ",");
	do
	{
		unsigned departure = strtoul(token, NULL, 10);
		buses[amount++] = departure;
	}
	while ((token = strtok(NULL, ",")));
	
	unsigned bus_id = 0;
	unsigned wait_time = 0;
	unsigned long long depart_time = time;
	while (!bus_id)
	{
		for (unsigned i = 0; i < amount; i++)
		{
			if (buses[i] == 0)
				continue;	// Skip buses out of service
			if (depart_time % buses[i] == 0)
			{
				bus_id = buses[i];
				wait_time = depart_time - time;
			}
		}
		depart_time++;
	}
	printf("Part 1 result: %d\n", bus_id * wait_time);
	depart_time = buses[0];	// Start at the first time-stamp possible
	// TODO: This is a brute force and not a good way to do it
	// can not get to the part2 solution like this
	while (true)
	{
		bool iteration = true;
		for (unsigned id = 0; id < amount; id++)
		{
			if (buses[id] == 0)
				continue;
			if ((depart_time + id) % buses[id] != 0)
				iteration = false;
		}
		if (iteration)
			break;
		depart_time += buses[0];	// Only check first bus time-stamps
	}
	printf("Part 2 result: %lld\n", depart_time);
	fclose(file);
	return EXIT_SUCCESS;
}
