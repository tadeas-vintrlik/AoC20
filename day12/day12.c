#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 800	// Maximum amount of movements
#define LINE 10
#define FULL_ANGLE 360
#define ABS(x) (x > 0 ? x : -(x))

typedef struct
{
	char dir;	// Direction of movement
	unsigned dist;	// Distance of the movement
} move_t;

typedef struct
{
	unsigned amount;	// Amount of elements in the vector
	move_t value[MAX];
} vector_t;

typedef enum {N = 0, E = 90, S = 180, W = 270 } dir_e;

typedef struct
{
	dir_e dir;	// The direction the ship is currently facing
	int east;	// Current coordinate, negative mean west
	int north;	// Current coordinate, negative mean south
} ship_t;

typedef struct
{
	// Same as ship but does not have a direction
	int east;
	int north;
} waypoint_t;

void load_move(FILE *file, vector_t *vector)
{
	vector->amount = 0;
	char line[LINE];
	while (fgets(line, LINE, file))
	{
		move_t new;
		new.dir = line[0];	// First character will be the direction
		int length = strlen(line) + 1;	// With null terminator as well
		for (int i = 1; i < length; i++)
			line[i-1] = line[i];
		new.dist = strtoul(line, NULL, 10);	// Can be NULL since we know input
		vector->value[vector->amount++] = new;
	}
}

// This is not particularly pretty, but boy is is fast
void process_movement(move_t move, ship_t *ship)
{
	int rotation = 0;
	switch (move.dir)
	{
	case 'N':
		ship->north += move.dist;
		break;
	case 'E':
		ship->east += move.dist;
		break;
	case 'S':
		ship->north -= move.dist;
		break;
	case 'W':
		ship->east -= move.dist;
		break;
	case 'F':
		switch (ship->dir)
		{
		case N:
			ship->north += move.dist;
			break;
		case E:
			ship->east += move.dist;
			break;
		case S:
			ship->north -= move.dist;
			break;
		case W:
			ship->east -= move.dist;
			break;
		}
		break;
	case 'L':
		rotation = ship->dir;
		rotation -= move.dist;
		while (rotation < 0)
			rotation += FULL_ANGLE;
		ship->dir = (dir_e) rotation;
		break;
	case 'R':
		rotation = ship->dir;
		rotation += move.dist;
		while (rotation >= 360)
			rotation -= FULL_ANGLE;
		ship->dir = (dir_e) rotation;
		break;
	}
}

void process_movement_part2(move_t move, ship_t *ship, waypoint_t *waypoint)
{
	unsigned temp = 0;
	switch (move.dir)
	{
	case 'N':
		waypoint->north += move.dist;
		break;
	case 'E':
		waypoint->east += move.dist;
		break;
	case 'S':
		waypoint->north -= move.dist;
		break;
	case 'W':
		waypoint->east -= move.dist;
		break;
	case 'F':
		ship->east += move.dist * waypoint->east;
		ship->north += move.dist * waypoint->north;
		break;
	case 'L':
		for (unsigned i = 0; i < move.dist; i += 90)
		{
			temp = waypoint->east;
			waypoint->east = - waypoint->north;
			waypoint->north = temp;
		}
		break;
	case 'R':
		for (unsigned i = 0; i < move.dist; i += 90)
		{
			temp = waypoint->east;
			waypoint->east = waypoint->north;
			waypoint->north = - temp;
		}
		break;
	}
}

int main(void)
{
	vector_t vector;
	FILE *file = fopen("day12.dat", "r");
	if (!file)
	{
		fprintf(stderr, "File could not be opened!\n");
		return EXIT_FAILURE;
	}
	load_move(file, &vector);

	ship_t ship = { .dir = E, .east = 0, .north = 0 };
	for (unsigned i = 0; i < vector.amount; i++)
		process_movement(vector.value[i], &ship);
	printf("Part 1 result: %d\n", ABS(ship.north) + ABS(ship.east));

	ship_t ship_2 = { .dir = E, .east = 0, .north = 0 };
	waypoint_t waypoint = { .east = 10, .north = 1 };
	for (unsigned i = 0; i < vector.amount; i++)
		process_movement_part2(vector.value[i], &ship_2, &waypoint);
	printf("Part 2 result: %d\n", ABS(ship_2.north) + ABS(ship_2.east));
	return EXIT_SUCCESS;
}
