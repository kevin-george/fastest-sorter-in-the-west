CC=gcc-7
CFLAGS=-Wall -pedantic -std=c99
all:sort read_machine
sort:sort_in_memory.c
	$(CC) $(CFLAGS) $^ -o $@
test_read_speeds: read_machine.c microtime.c
	$(CC) $(CFLAGS) $^ -o $@
