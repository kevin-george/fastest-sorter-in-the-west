#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "microtime.h"

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

int main(int argc, char **argv) {

  FILE *fdr = fopen("../../cop5570a/test0", "rb");
  //if(fread((void *)array, sizeof(long int), SIZE, fdr) != SIZE) exit(1);
  double rtime1 = microtime();
  int fsize = fread((void *)array, sizeof(long int), SIZE, fdr);
  double rtime2 = microtime();
  printf("fsize:%d time:%f\n", fsize, (rtime2-rtime1)/1000000);

  int child, listen_fd, sockfd;
  struct sockaddr_in cli_addr, serv_addr;
  socklen_t cli_len;

  listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons((short)atoi(argv[2]));

  bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

  if ( (child = fork()) == 0) {
    double time1, time2, read_time;
    char cmd[500];

    sprintf(cmd, "scp -r ../Final cop5570e@c5570-2.cs.fsu.edu:~/Final");
    if (system(cmd) == -1) exit(EXIT_FAILURE);
    sprintf(cmd, "ssh -q cop5570e@c5570-2.cs.fsu.edu 'cd Final && ./read_machine %s %s'", argv[1], argv[2]);
    if (system(cmd) == -1) exit(EXIT_FAILURE);

    sockfd = accept(listen_fd, (struct sockaddr *)(&cli_addr), &cli_len); 

    time1 = microtime();
    if (write(sockfd, (void*)array, sizeof(long int) * SIZE) == -1) exit(EXIT_FAILURE);
    time2 = microtime();

    if (read(sockfd, (void*)&read_time, sizeof(read_time)) == -1) exit(EXIT_FAILURE);
    if (system("ssh -q cop5570e@c5570-2.cs.fsu.edu 'rm -rf Final'") == -1) exit(EXIT_FAILURE);

    printf ("Time:%f\n", read_time+(time2-time1));

    exit(0);
  } else if (child < 0) {
    exit(EXIT_FAILURE);
  }

  wait(NULL);

  

  //quick_sort(0, SIZE - 1);
  //insertion_sort(SIZE);
  fclose(fdr);
  //if(fwrite((void *)array, sizeof(long int), SIZE, fdw) != SIZE) exit(1);
  return 0;
}  
