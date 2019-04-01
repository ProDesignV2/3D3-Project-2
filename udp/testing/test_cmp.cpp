#include "helper.h"
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

int main(){
	// Testing compare function
	std::cout << "Hello!\n";
	// Create variables to be tested
	struct addrinfo *test1 = get_address("2241:dc8::0:2","4567");
	struct addrinfo *test2 = get_address("2241:dc8::0:2","4567");
	printf("socklen_t : [%u]", test1->ai_addrlen);
	/*
	struct addrinfo *test3 = get_address("127.0.0.1","4567");
	struct addrinfo *test4 = get_address("127.0.0.1","4567");
	struct addrinfo *test5 = get_address("127.0.0.1","4567");
	struct addrinfo *test6 = get_address("127.0.0.1","4567");
	*/
	std::cout << "Goodbye!\n";
	// Perform tests
	if(compare_sockaddr(test1->ai_addr, test2->ai_addr)){ std::cout << "true\n"; }
	else{ std::cout << "false\n"; }
	return 0;
}
