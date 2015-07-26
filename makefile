CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -O3
all:singlechunk_quicksort singlechunk_hybrid singlechunk_hybrid_mmap singlechunk_hybrid_travis
singlechunk_quicksort:singlechunk_quicksort.c
	$(CC) $(CFLAGS) $^ -o $@
singlechunk_hybrid:singlechunk_hybrid.c microtime.c
	$(CC) $(CFLAGS) $^ -o $@
singlechunk_hybrid_mmap:singlechunk_hybrid_mmap.c
	$(CC) $(CFLAGS) $^ -o $@
singlechunk_hybrid_travis:singlechunk_hybrid_travis.c microtime.c
	$(CC) $(CFLAGS) $^ -o $@
read_machine: microtime.c
	$(CC) $(CFLAGS) -c microtime.c read_machine.c -o read_machine
