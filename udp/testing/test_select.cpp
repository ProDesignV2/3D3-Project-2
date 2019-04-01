#include <sys/types.h>
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

#define BUF_SIZE 1024

int main(){
	printf("Starting...\n");
	struct addrinfo hints, *res;
	struct sockaddr_storage their_addr;
	socklen_t addr_size = sizeof their_addr;
	char buf[BUF_SIZE];
	int n_bytes, count = 0;
	memset(&hints,0,sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo(NULL,"9999",&hints,&res);
	printf("socklen_t : [%u]\n",res->ai_addrlen);
	int sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	bind(sockfd,res->ai_addr,res->ai_addrlen);
	freeaddrinfo(res);
	struct timeval tv;
	fd_set master,readfds;
	FD_ZERO(&master);
	FD_ZERO(&readfds);
	FD_SET(sockfd,&master);
	while(1){
		readfds = master;
		tv.tv_sec = 10;
		tv.tv_usec = 0;
		printf("Select-ing... [%d]\n",count++);
		select(sockfd+1,&readfds,NULL,NULL,&tv);
		printf("Timeout : %lu sec %lu usec\n",tv.tv_sec,tv.tv_usec);
		if(!FD_ISSET(sockfd,&readfds)){ continue; }
		memset(buf,0,BUF_SIZE);
		n_bytes = 0;
		printf("Receiving...\n");
		n_bytes = recvfrom(sockfd,buf,BUF_SIZE,0,(struct sockaddr *)&their_addr,&addr_size);
		// buf[n_bytes] = '\0';
		printf("%s",buf);
	}
	return 0;
}
