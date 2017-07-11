#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define SIZE 1073741824
#define SMALL 10 

long int array[SIZE];

void insertion_sort(long int q) {
  for(int i = 1; i < q; i++) {
    int j = i;
    int x = array[i];
    while((j > 0) && (array[j - 1] > x)) {
      array[j] = array[j - 1];
      j--;
    }
    array[j] = x;
  }
}

void swap(long int* a,long int* b) {
  long int temp;
  temp = *a;
  *a = *b;
  *b = temp;
}

long int partition(long int p,long int q) {
  long int pivot_index, pivot_value;
  if(q - p > 40) {
    //If large subarays, use median of three
    long int a = array[p];
    long int b = array[q];
    long int c = array[(p + q)/2];
    pivot_value = a > b ? (c > a ? a : (c < b ? b : c)) : (c > b ? b : (c < a ? c : a));
    pivot_index = pivot_value == a ? p : (pivot_value == b ? q : ((p + q)/2));
  } else {
    //Else use middle element
    pivot_index = (p + q)/2;
    pivot_value = array[pivot_index];
  }

  //Storing low index
  long int i = p - 1;

  //Getting the pivot out of the way
  if(pivot_index != q)
    swap(&array[pivot_index], &array[q]);

  for(long int j = p; j < q; j++) {
    if(array[j] < pivot_value) {
      i++;
      if(i != j)
        swap(&array[i], &array[j]);
    }
  }
  if((i + 1) != q)
    swap(&array[i + 1], &array[q]);
  return i + 1;
}

void quick_sort(long int p,long int q) {
  if((q - p) < SMALL)
    return;
  else {
    long int j = partition(p, q);
    quick_sort(p, j - 1);
    quick_sort(j + 1, q);
  }
}

int main() {
  FILE *fdr = fopen("../cop5570a/test0", "rb");
  if(fread((void *)array, sizeof(long int), SIZE, fdr) != SIZE) exit(1);
  quick_sort(0, SIZE - 1);
  insertion_sort(SIZE);
  fclose(fdr);
  FILE *fdw = fopen("chunk0", "wb");
  if(fwrite((void *)array, sizeof(long int), SIZE, fdw) != SIZE) exit(1);
  fclose(fdw);
  return 0;
}  
