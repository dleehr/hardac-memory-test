CC=gcc

memory-test: memory-test.c
	$(CC) -std=c99 -o memory-test memory-test.c
