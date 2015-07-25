#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define SIZE 1073741824
#define SMALL 10 

long int *array;

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
  int fd = open("../cop5570a/test0", O_RDONLY);
  //We are reading chunks in 8GB at a time
  size_t size = SIZE * sizeof(long int);
  array = (long int *)mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
  
  quick_sort(0, SIZE - 1);
  insertion_sort(SIZE);
  
  munmap(array, size);
  close(fd);
  
  fd = open("chunk0", O_RDWR | O_CREAT, (mode_t)0600);
  lseek(fd, size + 1, SEEK_SET);
  if(write(fd, "", 1) == -1) exit(1);
  long int *map = (long int *)mmap(NULL, size, PROT_WRITE, MAP_PRIVATE, fd, 0);  
  for(size_t i = 0; i < SIZE; i++) {
    map[i] = array[i];
  }
  msync(map, size, MS_SYNC); 
  munmap(map, size);
  close(fd);
  
  return 0;
}  
