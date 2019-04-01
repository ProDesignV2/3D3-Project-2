// Helper functions definitions

#include "helper.h"

#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <string>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <vector>

#include <iostream>

void *
get_in_addr(struct sockaddr *sa)
{
	if(sa->sa_family == AF_INET){
		// Return IPV4 address
		struct sockaddr_in  *sa_in = (struct sockaddr_in *)sa;
		return &(sa_in->sin_addr);
	}
	// Return IPV6 address
	struct sockaddr_in6 *sa_in6 = (struct sockaddr_in6 *)sa;
	return &(sa_in6->sin6_addr);
}

unsigned short int
get_in_port(struct sockaddr *sa)
{
	if(sa->sa_family == AF_INET){
		// Return IPV4 port
		struct sockaddr_in *sa_in = (struct sockaddr_in *)sa;
		return sa_in->sin_port;
	}
	// Return IPV6 port
	struct sockaddr_in6 *sa_in6 = (struct sockaddr_in6 *)sa;
	return sa_in6->sin6_port;
}

bool
compare_sockaddr(struct sockaddr *sa_a, struct sockaddr *sa_b)
{
	// Check family, port and address (doesn't check other IPv6 stuff)

	// Compare IP version
	if(sa_a->sa_family != sa_b->sa_family){ return false; }
	// Compare port number
	if(get_in_port(sa_a) != get_in_port(sa_b)){ return false; }
	// Compare IP address for given version
	switch(sa_a->sa_family){
		case AF_INET:
			{
				struct in_addr *temp_a = (struct in_addr *)get_in_addr(sa_a);
				struct in_addr *temp_b = (struct in_addr *)get_in_addr(sa_b);
				if(temp_a->s_addr != temp_b->s_addr){ return false; }
			}
			break;
		case AF_INET6:
			{
				struct in6_addr *temp_a = (struct in6_addr *)get_in_addr(sa_a);
				struct in6_addr *temp_b = (struct in6_addr *)get_in_addr(sa_b);
				for(int i = 0; i < 16; i++){
					if((temp_a->s6_addr)[i] != (temp_b->s6_addr)[i]){ return false; }				
				}
			}
			break;
		default:	
			perror("router : compare");
			exit(0);
	}

	// All checked fields are equal	
	return true;
}

bool
add_new_addr(struct sockaddr *sa, std::vector<struct sockaddr *> &sa_list)
{
	// Flag for checking if sockaddr is in list
	bool inList = false;

	// Check if new sockaddr
	for(auto address : sa_list){
		// If sockaddr already in list, break checking loop
		if(compare_sockaddr(address, sa)){ inList = true; break; }
	}

	// Add sockaddr to list if new
	if(!inList){ 
		// Create new pointer and copy sockaddr struct over
		struct sockaddr *copy_addr = new struct sockaddr;	
		*copy_addr = *sa;
		// Add copied struct to list
		sa_list.push_back(copy_addr);
		// Return that new sockaddr was added 
		return true;
	}

	// No new sockaddr was added
	return false;
}

int
send_all(int send_fd, const char *buf, int *len, struct sockaddr *to, socklen_t to_len)
{
	int num_bytes, bytes_left = *len, bytes_sent = 0;

	while(bytes_sent < *len){
		num_bytes = sendto(send_fd, buf + bytes_sent, bytes_left, 0, to, to_len);
		if(num_bytes == -1){ break; }
		bytes_sent += num_bytes;
		bytes_left -= num_bytes;
	}
	
	// Put number of sent bytes into original length variable
	*len = bytes_sent;

	// Return success outcome
	return num_bytes == -1 ? -1 : 0;
}

int
setup_socket_udp(const char *port)
{
	struct addrinfo *router_info, *res_point;
	int router_fd, yes = 1;

	// Get address structs based on host IP and given port
	router_info = get_address(NULL, port);
	
	// Loop through getaddrinfo results for available struct  
	for(res_point = router_info; res_point != NULL; res_point = res_point->ai_next){
		
		// Create a socket for the router using UDP (Datagram)
		if((router_fd = socket(res_point->ai_family, res_point->ai_socktype, res_point->ai_protocol)) == -1) {
			perror("router : socket");
			continue;
		}		

		// Allow others to reuse the socket
		if (setsockopt(router_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			// Why is this an exit call ?
			exit(1);
		}

		// Bind host address to socket
		if (bind(router_fd, res_point->ai_addr, res_point->ai_addrlen) == -1) {
			close(router_fd);
			perror("router : bind");
			continue;
		}

		break;
	}
	
	// Free addrinfo struct
	freeaddrinfo(router_info);	
	
	// If no working address struct is found
	if(res_point == NULL){
		fprintf(stderr, "router : failed to bind\n");
		exit(2);
	}

	return router_fd; 
}

struct addrinfo *
get_address(const char *addr, const char *port)
{	
	struct addrinfo hints, *router_info;
	int gai_result;

	// Set parameters for address structs
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	// Get available address structs based on host IP
	if((gai_result = getaddrinfo(addr, port, &hints, &router_info)) != 0)
	{
		fprintf(stderr, "getaddrinfo : %s\n", gai_strerror(gai_result));
		exit(0);
	}

	return router_info;
}

bool
wait_for_packet(int router_fd, struct timeval *tv)
{
	// Create new file descriptor set and add router socket
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(router_fd, &readfds);
	
	// Wait for incoming data
	if(select(router_fd + 1, &readfds, NULL, NULL, tv) == -1){
		perror("select");
		exit(0);
	}

	// Check if is data or timeout
	if(FD_ISSET(router_fd, &readfds)){ return true; }
	return false;	
}

void
countdown_timeval(std::clock_t curr_clock, struct timeval *tv)
{
	// Get time from clock ticks
	double time = (double)curr_clock / (double)CLOCKS_PER_SEC;
	// Extract seconds
	unsigned long seconds = (unsigned long)time;
	// Remove integer part and convert to micro-seconds
	time = (time - seconds) * 1000000;
	// Save micro-seconds
	unsigned long useconds = (unsigned long)(time);
	tv->tv_sec -= (time_t)seconds;
	tv->tv_usec -= (suseconds_t)useconds;	
}
