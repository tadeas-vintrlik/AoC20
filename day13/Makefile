FILE=day13
FLAGS=-std=c99 -Wall -Werror -Wextra -pedantic
OUTPUT=main
CC=cc
all: $(FILE).c
	$(CC) $(FLAGS) -o $(OUTPUT) $(FILE).c

debug: $(FILE).c
	$(CC) $(FLAGS) -g -o $(OUTPUT) $(FILE).c

fast: $(FILE).c
	$(CC) $(FLAGS) -O3 -o $(OUTPUT) $(FILE).c
