#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "helper.h"

#define DEFAULT_PORT "4000"
#define BUFFER_SIZE 1024

int
main(int argc, char *argv[])
{
	int router_fd, n_bytes;
	struct sockaddr_storage their_addr;
	socklen_t their_addr_size;
	char ipstr[INET6_ADDRSTRLEN], buf[BUFFER_SIZE];
	std::string router_port = DEFAULT_PORT;

	
	
	//Modified argument parser goes here
	
	
	// Set command argument as router port if entered
	//switch(argc){ 
	//	case 1: break;
	//	case 2: router_port = argv[1]; break;
	//	default:		
	//		fprintf(stderr, "usage: ./router [port]\n"); 
	//		exit(0); 
	//}

	
	
	
	// Setup up UDP socket at given port
	router_fd = setup_socket_udp(router_port.c_str());	
	
	// Wait for connections and deal with them
	while(1){
	
		struct addrinfo *router_info;

		// Get info for other router
		router_info = get_address("localhost","4000");
	
		// Create message and set byte count
		char testing[] = {'G','o','o','d','b','y','e',' ','A','l','e','x','\0'};
		n_bytes = 13;

		printf("router : sending test data...\n");
		std::cout << "data : [" << testing << "]\n";
		
		if (send_all(router_fd, testing, &n_bytes, router_info->ai_addr, router_info->ai_addrlen) == -1) {
			perror("send_all");
			continue;
		}

		// Reset buffer
		memset(&buf, 0, sizeof buf);
		n_bytes = 0;	
		their_addr_size = sizeof their_addr;

		n_bytes = recvfrom(router_fd, buf, sizeof buf, 0,
				   (struct sockaddr *)&their_addr, &their_addr_size);
		if(n_bytes == -1) { perror("recvfrom"); exit(0); }

		// Print the other router's address and port details
		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), ipstr, sizeof ipstr);
		std::cout << "router : received " << n_bytes << " bytes from " << ipstr << ":" <<
		ntohs(get_in_port((struct sockaddr *)&their_addr)) << std::endl;

		// Print data
		std::cout << buf << std::endl;
		// if(n_bytes < BUFFER_SIZE){ buf[n_bytes] = '\0'; std::cout << buf << std::endl; }
	
		/*	
		testing = new char[resp.len_msg()];
		strcpy(testing, resp.get_msg());
		for(int i = 0; i < resp.len_msg(); i++){
			if(testing[i] < 32){ testing[i] = '#'; }
		}
		std::cout << testing << std::endl;
		*/

		break;	
	}
	
	return 0;
}
