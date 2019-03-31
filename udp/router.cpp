#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include <sys/types.h>
#include <netdb.h>

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

		// Reset buffer
		memset(&buf, 0, sizeof buf);
		n_bytes = 0;	
		their_addr_size = sizeof their_addr;

		printf("router : waiting for connections...\n");

		/*
		if((n_bytes = recvfrom(router_fd, buf, sizeof buf, 0, (struct sockaddr *)&their_addr, &their_addr_size)) == -1)
		{ 
			perror("recvfrom");
			exit(0); 
		}
		
		// Print the other router's address and port details
		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), ipstr, sizeof ipstr);
		std::cout << "router : received data from " << ipstr << ":" <<
		ntohs(get_in_port((struct sockaddr *)&their_addr)) << std::endl;

		// Print number of bytes and data if no buffer overflow	
		std::cout << "router : " << n_bytes << " received\n";
		if(n_bytes < BUFFER_SIZE){ buf[n_bytes] = '\0'; std::cout << buf << std::endl; }
		*/

		/*
		char *testing = new char[req.len_msg()];
		strcpy(testing, req.get_msg());
		for(int i = 0; i < req.len_msg(); i++){
			if(testing[i] < 32){ testing[i] = '#'; }
		}
		std::cout << "[" << req.len_msg() << "]\n" << testing << std::endl;
		*/
		
		struct addrinfo *temp = get_address("localhost","4000");		

		// Create message and set byte count
		char testing[] = {'H','e','l','l','o',' ','E','o','i','n','\0'};
		n_bytes = 11;
		std::cout << "data : [" << testing << "]\n";
				
		// if (send_all(router_fd, testing, &n_bytes, (struct sockaddr *)&their_addr, their_addr_size) == -1) {
		if (send_all(router_fd, testing, &n_bytes, temp->ai_addr, temp->ai_addrlen) == -1) {
			perror("send_all");
			// continue;
			break;
		}

		break;
	}
	
	return 0;
}
