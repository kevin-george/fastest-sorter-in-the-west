#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define SMALL 16
#define BUFFER_SIZE 8192

void insertion_sort(long *array, long q) {
  for(int i = 1; i < q; i++) {
    long j = i;
    long x = *(array + i);
    while((j > 0) && (*(array + (j - 1)) > x)) {
      *(array + j) = *(array + (j - 1));
      j--;
    }
    *(array + j) = x;
  }
}

void swap(long *a, long *b) {
  long int temp;
  temp = *a;
  *a = *b;
  *b = temp;
}

long int partition(long *array, long p, long q) {
  long pivot_index, pivot_value;
  if(q - p > 40) {
    //If large subarays, use median of three
    long a = *(array + p);
    long b = *(array + q);
    long c = *(array + ((p + q)/2));
    pivot_value = a > b ? (c > a ? a : (c < b ? b : c)) : (c > b ? b : (c < a ? c : a));
    pivot_index = pivot_value == a ? p : (pivot_value == b ? q : ((p + q)/2));
  } else {
    //Else use middle element
    pivot_index = (p + q)/2;
    pivot_value = *(array + pivot_index);
  }

  //Storing low index
  long i = p - 1;

  //Getting the pivot out of the way
  if(pivot_index != q)
    swap(array + pivot_index, array + q);

  for(long j = p; j < q; j++) {
    if(*(array + j) < pivot_value) {
      i++;
      if(i != j)
        swap(array + i, array + j);
    }
  }
  if((i + 1) != q)
    swap(array + i + 1, array + q);
  return i + 1;
}

void quick_sort(long *array, long p, long q) {
  if((q - p) < SMALL)
    return;
  else {
    long j = partition(array, p, q);
    quick_sort(array, p, j - 1);
    quick_sort(array, j + 1, q);
  }
}

int main(int argc, char *argv[]) {
  if(argc < 2) {
    printf("\n Usage is ./sort_fast <input_file>");
  }
  FILE *fdr = fopen(argv[1], "rb");

  // Determine the size of the file
  fseek(fdr, 0, SEEK_END);
  long size = ftell(fdr);
  fseek(fdr, 0, SEEK_SET);

  // Find number of elements within file
  size_t no_of_ints = (size % sizeof(long) ? 1 : 0) + (size / sizeof(long));

  // Find number of reads required to read entire file
  size_t no_of_reads = (size % BUFFER_SIZE ? 1 : 0) + (size / BUFFER_SIZE);

  long *array;
  if(!(array = malloc(no_of_ints * sizeof(long)))) {
    fprintf(stderr, "\nFailed to allocate memory!!");
    return 1;
  }
  fread(array, BUFFER_SIZE, no_of_reads, fdr);
  fclose(fdr);

  // Actual sorting
  quick_sort(array, 0, no_of_ints - 1);
  insertion_sort(array, no_of_ints);

  //Writing results to file
  FILE *fdw = fopen("sorted_file.bin", "wb");
  fwrite(array, sizeof(long), no_of_ints, fdw);
  fclose(fdw);

  free(array);
  return 0;
}
