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
#include <iostream>
#include <list>
#include <string>

#include "httpmsg.h"
#include "helper.h"

#define DEFAULT_PORT "4000"
#define BUFFER_SIZE 1024

int
main(int argc, char *argv[])
{
	struct addrinfo hints, *router_info, *res_point;
	int router_fd, gai_result, n_bytes, yes = 1;
	struct sockaddr_storage their_addr;
	socklen_t their_addr_size;
	char ipstr[INET6_ADDRSTRLEN], buf[BUFFER_SIZE];
	std::string router_port;	
	
	router_port = DEFAULT_PORT;

	// Set command argument as router port if entered
	switch(argc){ 
		case 1: break;
		case 2: router_port = argv[1]; break;
		default:		
			fprintf(stderr, "usage: ./router [port]\n"); 
			exit(0); 
	}

	// Set parameters for address structs
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	// Get available address structs based on host IP
	if((gai_result = getaddrinfo(NULL,
				     router_port.c_str(),
			 	     &hints,
			   	     &router_info)) != 0){
		fprintf(stderr, "getaddrinfo : %s\n", gai_strerror(gai_result));
		exit(0);
	}
	
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
	
	// Free addrinfo struct - Should res_point be freed too ?
	freeaddrinfo(router_info);	
	
	// If no working address struct is found
	if(res_point == NULL){
		fprintf(stderr, "router : failed to bind\n");
		exit(2);
	} 
	
	// Reap all dead processes here ?
	
	// Wait for connections and deal with them
	while(1){

		// Reset buffer
		memset(&buf, 0, sizeof buf);
		n_bytes = 0;	
		
		// Create empty HTTP request
		HTTP_Request req;

		printf("router : waiting for connections...\n");

		while(!(req.append(buf, n_bytes))){
			n_bytes = recvfrom(router_fd, buf, sizeof buf, 0,
				           (struct sockaddr *)&their_addr, &their_addr_size);
			if(n_bytes == -1) { perror("recvfrom"); exit(0); }
		}

		// Print the other router's address and port details
		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), ipstr, sizeof ipstr);
		std::cout << "router : received data from " << ipstr << ":" <<
		ntohs(get_in_port((struct sockaddr *)&their_addr)) << std::endl;

		char *testing = new char[req.len_msg()];
		strcpy(testing, req.get_msg());
		for(int i = 0; i < req.len_msg(); i++){
			if(testing[i] < 32){ testing[i] = '#'; }
		}
		std::cout << "[" << req.len_msg() << "]\n" << testing << std::endl;
				
		// Create HTTP response
		HTTP_Response resp;

		if(req.is_error()){ resp.add_header("HTTP/1.0 400 Bad request"); }
		else{
			resp.add_header("HTTP/1.0 200 OK");
			resp.add_header("Content-Encoding: binary");
			resp.add_header("Content-Type: text/plain");
		}

		n_bytes = resp.len_msg();

		testing = new char[resp.len_msg()];
		strcpy(testing, resp.get_msg());
		for(int i = 0; i < resp.len_msg(); i++){
			if(testing[i] < 32){ testing[i] = '#'; }
		}
		std::cout << testing << std::endl;

		if (send_all(router_fd, resp.get_msg(), &n_bytes, (struct sockaddr *)&their_addr, their_addr_size) == -1) {
			perror("send_all");
			continue;
		}
	}
	
	return 0;
}
