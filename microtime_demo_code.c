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
