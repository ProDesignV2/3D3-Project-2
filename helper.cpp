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
