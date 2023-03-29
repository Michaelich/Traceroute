#include "traceroute.h"

/*
 * Send 3 ICMP packets to <dest_addr>. Modify ttl accordingly.
 */
int sender(int sockfd, int ttl, int pid, struct sockaddr_in dest_addr)
{
	for (int i = 0; i < 3; i++)
	{
		struct icmp header;
		header.icmp_type = ICMP_ECHO;
		header.icmp_code = 0;
		header.icmp_hun.ih_idseq.icd_id = pid;
		header.icmp_hun.ih_idseq.icd_seq = ttl;
		header.icmp_cksum = 0;
		header.icmp_cksum = compute_icmp_checksum((u_int16_t*)&header, sizeof(header));
		if(setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0)
		{
			fprintf(stderr, "Setsockopt error: %s\n", strerror(errno)); 
			return EXIT_FAILURE;
		}
		if(sendto(sockfd, &header, sizeof(header), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) < 0)
		{
			fprintf(stderr, "Send error: %s\n", strerror(errno)); 
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

