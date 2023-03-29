#include "traceroute.h"


/*
 * Wait for ICMP packets send by sender. Stop after 1 sec, or when all packets return.
 * Print IP address reached and average time it took ("???" when not all packets recived in given time) 
 */
int reciver(int sockfd, int ttl, int pid)
{
	int counter = 0;
	char sender_ip_strings[3][20]; 
	int sender_ip_count = 0;
	int success = 0;
	fd_set descriptors;
	FD_ZERO (&descriptors);
	FD_SET (sockfd, &descriptors);
	struct timeval tv;
	tv.tv_sec = 1; 
	tv.tv_usec = 0;
	struct timeval tvS;
	struct timeval tvK; 
	double time_sum = 0;
	if(gettimeofday(&tvS, NULL) < 0)
	{
		fprintf(stderr, "Gettimeofday error: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
	}
	double time_start = (tvS.tv_sec) * 1000 + (tvS.tv_usec) / 1000;
	int ready = 1;
	while(ready)
	{
		ready = select (sockfd+1, &descriptors, NULL, NULL, &tv);
		if(ready < 0)
		{
			fprintf(stderr, "Select error: %s\n", strerror(errno)); 
			return EXIT_FAILURE;
		}
		for(int i = 0; i < ready; i++)
		{
			struct sockaddr_in sender;
			socklen_t sender_len = sizeof(sender);
			u_int8_t buffer[IP_MAXPACKET];
			ssize_t packet_len = recvfrom(sockfd, buffer, IP_MAXPACKET, MSG_DONTWAIT, (struct sockaddr*)&sender, &sender_len);
			if(packet_len < 0)
			{
				fprintf(stderr, "Recvfrom error: %s\n", strerror(errno)); 
				return EXIT_FAILURE;
			}
			// Extract ICMP
			struct ip* ip_header = (struct ip*) buffer;
			u_int8_t* icmp_packet = buffer + 4 * ip_header->ip_hl;
			struct icmp* icmp_header = (struct icmp*) icmp_packet;
			if(icmp_header->icmp_type == ICMP_TIME_EXCEEDED)
			{
				// ICMP DATA is 64 bit further
				ip_header = (struct ip *)((uint8_t *)icmp_header + 8);
				icmp_packet = (uint8_t *)ip_header + 4 * (*ip_header).ip_hl;
				icmp_header = (struct icmp *)icmp_packet;
			}
			if(icmp_header->icmp_hun.ih_idseq.icd_id == pid && icmp_header->icmp_hun.ih_idseq.icd_seq == ttl)
			{
				// Destination reached
				if (icmp_header->icmp_type == ICMP_ECHOREPLY) success = 1;
				char sender_ip_str[20];
				if(inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str)) == NULL)
				{
					fprintf(stderr, "Inet_ntop error: %s\n", strerror(errno)); 
					return EXIT_FAILURE;
				}
				if (sender_ip_count == 0)
				{
					strcpy(sender_ip_strings[sender_ip_count], sender_ip_str);
					sender_ip_count += 1;
				}
				else
				{
					for(int i=0; i < sender_ip_count; i++)
					{
						if(strcmp(sender_ip_str, sender_ip_strings[i]) != 0)
						{
							strcpy(sender_ip_strings[sender_ip_count], sender_ip_str);
							sender_ip_count += 1;
							break;
						}
					}
				}
				counter += 1;
				if(gettimeofday(&tvK, NULL) < 0)
				{
					fprintf(stderr, "Gettimeofday error: %s\n", strerror(errno)); 
					return EXIT_FAILURE;
				}
				time_sum += ((tvK.tv_sec) * 1000 + (tvK.tv_usec) / 1000) - time_start;
			}
		}
		if (counter >= 3) break;
		if(gettimeofday(&tvS, NULL) < 0)
		{
			fprintf(stderr, "Gettimeofday error: %s\n", strerror(errno)); 
			return EXIT_FAILURE;
		}
	}
	// Print results
	printf ("%d. ", ttl);
	for(int i = 0; i < sender_ip_count; i++)
	{
		printf("%s ",sender_ip_strings[i]);
	}
	if (counter == 0) printf("*\n");
	else if(counter < 2) printf("???\n");
	else printf("%fms\n", (time_sum / 3));
	return success;
}