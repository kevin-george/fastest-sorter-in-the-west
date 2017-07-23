CC=gcc-7
CFLAGS=-Wall -pedantic -std=c99
all:fast
fast:sort_in_memory.c
	$(CC) $(CFLAGS) $^ -o $@
create_tests:
	python3 generator.py test1
run_tests:
	./fast test1.bin
	diff test1_sorted.bin sorted_file.bin
test_read_speeds: read_machine.c microtime.c
	$(CC) $(CFLAGS) $^ -o $@
clean:
	rm fast
	rm *.bin

