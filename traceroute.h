#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <assert.h>

int sender(int sockfd, int ttl, int pid, struct sockaddr_in dest_addr);
int reciver(int sockfd, int ttl, int pid);
u_int16_t compute_icmp_checksum (const void *buff, int length);
void print_as_bytes (unsigned char* buff, ssize_t length);
