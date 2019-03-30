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
#include <ctime>
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
	std::clock_t timer;
	timeval tv, tv_copy;

	// Set command argument as router port if entered
	switch(argc){ 
		case 1: break;
		case 2: router_port = argv[1]; break;
		default:		
			fprintf(stderr, "usage: ./router [port]\n"); 
			exit(0); 
	}

	// Setup up UDP socket at given port
	router_fd = setup_socket_udp(router_port.c_str());	

	// Create empty vector for sockaddr structs
	std::vector<struct addrinfo *> other_routers;

	// Create and add some sample router structs
	other_routers.push_back(get_address("localhost","6000"));
	other_routers.push_back(get_address("localhost","7000"));
	other_routers.push_back(get_address("localhost","8000"));
	
	// Wait for connections and deal with them
	while(1){
	
		// Create message and set byte count
		char testing[] = {'G','o','o','d','b','y','e',' ','A','l','e','x','\0'};
		n_bytes = 13;

		printf("router : sending test data...\n");
		std::cout << "data : [" << testing << "]\n";
		
		for(auto router : other_routers){
			if (send_all(router_fd, testing, &n_bytes, router->ai_addr, router->ai_addrlen) == -1) {
				perror("send_all");
				continue;
			}
		}

		// Start timer for receiving data
		timer = std::clock();
		tv.tv_sec = TIMEOUT_SEC;
		tv.tv_usec = TIMEOUT_USEC;
		
		do
		{
			// Break and go to send routine if false (timeout)	
			tv_copy = tv;
			if(!wait_for_packet(router_fd, &tv_copy)){ break; }


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
			std::cout << "router : received " << n_bytes << " bytes from " << ipstr << ":" <<
			ntohs(get_in_port((struct sockaddr *)&their_addr)) << std::endl;

			// Print data
			std::cout << buf << std::endl;

			// Finish "stuff"


			// Subtract time elapsed from timeval struct
			countdown_timeval((std::clock() - timer), &tv);
			timer = std::clock();
		}
		while((tv.tv_sec > 0) && (tv.tv_usec > 0));

	}
	
	return 0;
}
