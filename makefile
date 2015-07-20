CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -O3
all:singlechunk_quicksort singlechunk_hybrid singlechunk_hybrid_mmap
singlechunk_quicksort:singlechunk_quicksort.c
	$(CC) $(CFLAGS) $^ -o $@
singlechunk_hybrid:singlechunk_hybrid.c
	$(CC) $(CFLAGS) $^ -o $@
singlechunk_hybrid_mmap:singlechunk_hybrid_mmap.c
	$(CC) $(CFLAGS) $^ -o $@

