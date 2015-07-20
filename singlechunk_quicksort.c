#include <stdio.h>
#include <stdlib.h>
#define SIZE 1073741824

long int array[SIZE]; 

void swap(long int* a,long int* b) {
  long int temp;
  temp = *a;
  *a = *b;
  *b = temp;
}

int partition(long int p,long int q) {
  //Randomly choosing pivot
  long int pivot_index = p + rand()%(q - p + 1);
  long int pivot_value = array[pivot_index];
  
  //Storing low index
  long int i = p - 1;
  
  //Getting the pivot out of the way
  swap(&array[pivot_index], &array[q]);

  for(long int j = p; j < q; j++) {
    if(array[j] < pivot_value) {
      i++;
      swap(&array[i], &array[j]);
    }
  }
  swap(&array[i+1], &array[q]);
  return i + 1;
}

void quick_sort(long int p,long int q) {
  if(p < q) {
    long int j = partition(p, q);
    quick_sort(p, j-1);
    quick_sort(j+1, q);
  }
}

int main() {
  FILE *fdr = fopen("../cop5570a/test0", "rb");
  if(fread((void *)array, sizeof(long int), SIZE, fdr) != SIZE) exit(1);
  quick_sort(0, SIZE - 1);
  FILE *fdw = fopen("chunk0", "wb");
  if(fwrite((void *)array, sizeof(long int), SIZE, fdw) != SIZE) exit(1);
  return 0;
}  
