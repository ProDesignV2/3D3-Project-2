#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

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
	timeval tv;

	
	
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
	router_fd = setup_socket_udp(argv[1]);	
	// router_fd = setup_socket_udp(router_port.c_str());	

	// Create empty vector for sockaddr structs
	std::vector<struct sockaddr *> other_routers;

	// Add routers specified in command line
	for(int port_arg = 2; port_arg < argc; port_arg++){
		other_routers.push_back(get_address("localhost", argv[port_arg])->ai_addr);
	}
	
	// Wait for connections and deal with them
	while(1){
	
		if(!other_routers.empty()){

			// Create message and set byte count
			memset(&buf, 0, sizeof buf);
			sprintf(buf, "I am a router, located at port %s", argv[1]);
			n_bytes = strlen(buf);

			printf("\nMessage [%dB]: %s\nRouter [%s] Sending to...\n\n", n_bytes, buf, argv[1]);
			
			for(auto router : other_routers){
				
				// Print the other router's address and port details
				inet_ntop(router->sa_family, get_in_addr(router), ipstr, sizeof ipstr);
				std::cout << ipstr << ":" << ntohs(get_in_port(router)) << std::endl;

				their_addr_size = sizeof their_addr;
				if (send_all(router_fd, buf, &n_bytes, router, their_addr_size) == -1) {
					perror("send_all");
					continue;
				}
			}
		}

		// Reset timeout for receiving data
		tv.tv_sec = TIMEOUT_SEC;
		tv.tv_usec = TIMEOUT_USEC;
		
		printf("\nRouter [%s]\nReceiving from...\n\n", argv[1]);

		do
		{
			// Break and go to send routine if false (timeout)	
			if(!wait_for_packet(router_fd, &tv)){ break; }


			// Read in available data from buffer and do stuff

			// Reset buffer
			memset(&buf, 0, sizeof buf);
			n_bytes = 0;	
			their_addr_size = sizeof their_addr;

			n_bytes = recvfrom(router_fd, buf, sizeof buf, 0,
					   (struct sockaddr *)&their_addr, &their_addr_size);
			if(n_bytes == -1) { perror("recvfrom"); exit(0); }

			// Print the other router's address and port details
			inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), ipstr, sizeof ipstr);
			std::cout << ipstr << ":" << ntohs(get_in_port((struct sockaddr *)&their_addr)) <<
			" [" << n_bytes << "B] " << buf << std::endl; 
		
			// Check if new router, and add to list	
			add_new_addr((struct sockaddr *)&their_addr, other_routers);
		}
		while((tv.tv_sec > 0) && (tv.tv_usec > 0));

	}
	
	return 0;
}
