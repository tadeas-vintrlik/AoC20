#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE 20
#define CMD_NAME 4
#define MAX_INS 700

typedef struct
{
	char name[CMD_NAME];
	int param;
	bool executed;
	int index;
} instruct_t;

typedef struct
{
	instruct_t ins[MAX_INS];
	int amount;
} instruct_array_t;

bool get_instr(FILE *file, char instr[MAX_LINE])
{
	if (!fgets(instr, MAX_LINE, file))
		return false;
	return true;
}

// Store instruction from input into the array
void parse_instr(char instr[MAX_LINE], instruct_array_t *ins_arr)
{
	char *token = strtok(instr, " ");
	strcpy(ins_arr->ins[ins_arr->amount].name, token);
	token = strtok(NULL, " ");
	ins_arr->ins[ins_arr->amount++].param = strtol(token, NULL, 10);
}

// Execute ins_arr return if terminated successfully (did not loop)
bool execute(instruct_array_t ins_arr, int *acc)
{
	int terminated = false;
	*acc = 0;
	for (int i = 0; i < ins_arr.amount && !ins_arr.ins[i].executed;)
	{
		// If last instruction the programme terminated
		if (i == ins_arr.amount - 1)
			terminated = true;
		ins_arr.ins[i].executed = true;
		if (strcmp(ins_arr.ins[i].name, "nop") == 0)
		{
			i++;
			continue;
		}
		else if(strcmp(ins_arr.ins[i].name, "acc") == 0)
		{
			*acc += ins_arr.ins[i].param;
			i++;
		}
		else // jmp
			i += ins_arr.ins[i].param;
	}
	return terminated;
}

bool is_jmp(instruct_array_t ins_arr, int i)
{
	return strcmp(ins_arr.ins[i].name, "jmp") == 0;
}

bool is_nop(instruct_array_t ins_arr, int i)
{
	return strcmp(ins_arr.ins[i].name, "nop") == 0;
}

// Replace one instruction in ins_arr that could be causing loop, return the
// index of element where the change was made
int fix_ins_arr(instruct_array_t *ins_arr, int skip)
{
	int suspects = 0;
	for (int i = 0; i < ins_arr->amount; i++)
	{
		if (!(is_jmp(*ins_arr, i) || is_nop(*ins_arr, i)))
			continue;

		// If suspect for looping
		if (is_nop(*ins_arr, i) && ins_arr->ins[i].param == 0)
			continue; // Skip nop 0 since it would create loop by itself
		suspects++;
		if (suspects <= skip)
			continue; // Skip how many were already tried
		if (is_jmp(*ins_arr, i))
		{
			// Replace jmp with nop
			strcpy(ins_arr->ins[i].name, "nop");
			return i;
		}
		if (is_nop(*ins_arr, i))
		{
			// Replace nop with jmp
			strcpy(ins_arr->ins[i].name, "jmp");
			return i;
		}
	}
	// Should not reach here
	return 0;
}

// Change back the instruction we tried to change to fix the loop
void revert_change(instruct_array_t *ins_arr, int change_index)
{
	if (is_jmp(*ins_arr, change_index))
		strcpy(ins_arr->ins[change_index].name, "nop");
	else
		strcpy(ins_arr->ins[change_index].name, "jmp");
}

int main(void)
{

	FILE *file = fopen("day8.dat", "r");
	if (!file)
	{
		fprintf(stderr, "File could not be opened!\n");
		return EXIT_FAILURE;
	}

	char instr[MAX_LINE];
	instruct_array_t ins_arr = { .amount = 0 };
	while (get_instr(file, instr))
		parse_instr(instr, &ins_arr);

	int acc = 0;
	int skip = 0;
	int change_index = 0;
	bool terminated = false;
	terminated = execute(ins_arr, &acc);
	printf("Acc when loop entered: %d\n", acc);
	while (!terminated)
	{
		change_index = fix_ins_arr(&ins_arr, skip);
		if (!execute(ins_arr, &acc))
			revert_change(&ins_arr, change_index);
		else
			terminated = true;
		skip++;
	}
	printf("Acc after fix: %d\n", acc);
	printf("Changed line: %d\n", change_index);
	return EXIT_SUCCESS;
}
