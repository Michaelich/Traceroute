#include "traceroute.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Wrong number of arguments\n"); 
		return EXIT_FAILURE;
	}
    // Read ip adress
	int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd < 0)
	{
		fprintf(stderr, "Socket error: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
	}
	struct sockaddr_in recipient;
	bzero(&recipient, sizeof(recipient));
	recipient.sin_port = htons(7);
	recipient.sin_family = AF_INET;
	if(inet_pton(AF_INET, argv[1], &(recipient.sin_addr)) <= 0)
	{
		fprintf(stderr, "Argument error: Not valid IP adress\n"); 
		return EXIT_FAILURE;
	}
	int pid = getpid() & 65535;
	// Send packets with sender, and wait for response in reciver
	for (int i = 1; i <= 30; i++) 
	{
		if(sender(sockfd, i, pid, recipient) == EXIT_FAILURE) return EXIT_FAILURE;
        if(reciver(sockfd, i, pid) == 1) return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}
//---Functions taken from lecture---

u_int16_t compute_icmp_checksum (const void *buff, int length)
{
	u_int32_t sum;
	const u_int16_t* ptr = buff;
	assert (length % 2 == 0);
	for (sum = 0; length > 0; length -= 2)
		sum += *ptr++;
	sum = (sum >> 16) + (sum & 0xffff);
	return (u_int16_t)(~(sum + (sum >> 16)));
}

void print_as_bytes (unsigned char* buff, ssize_t length)
{
	for (ssize_t i = 0; i < length; i++, buff++)
		printf ("%.2x ", *buff);	
}