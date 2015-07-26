#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "microtime.h"

#define SIZE 1073741824
#define SMALL 10

long int array[SIZE];

int main(int argc, char **argv) {
	double time1, time2;
	int r_port = atoi(argv[2]);
	int sockfd;
	struct sockaddr_in r_addr;
	
	memset(&r_addr, 0, sizeof(r_addr));
	r_addr.sin_family = AF_INET;
	r_addr.sin_port = htons((short)r_port);
	inet_pton(AF_INET, argv[1], &r_addr.sin_addr);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    connect(sockfd, (struct sockaddr*)&r_addr, sizeof(r_addr));

    time1 = microtime();
    if (read(sockfd, (void *)array, sizeof(long int)*SIZE) == -1) exit(EXIT_FAILURE);
    time2 = microtime();
    
    double elapsed = time2-time1;
    if (write(sockfd, (void *)&elapsed, sizeof(double)) == -1) exit(EXIT_FAILURE);

    return 0;
}