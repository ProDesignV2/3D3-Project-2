#ifndef HELPER_H
#define HELPER_H

// Helper functions for main program

#include <sys/socket.h>
#include "string.h"
#include <string.h>
#include <string>

#define TIMEOUT_SEC 6
#define TIMEOUT_USEC 500000
#define NUM 10

void *
get_in_addr(struct sockaddr *sa);

unsigned short int
get_in_port(struct sockaddr *sa);

bool
compare_sockaddr(struct sockaddr *sa_a, struct sockaddr *sa_b);

int
send_all(int send_fd, const char *buf, int *len, struct sockaddr *to, socklen_t to_len);

int
setup_socket_udp(const char *port);

struct addrinfo *
get_address(const char *addr, const char *port);

bool
wait_for_packet(int router_fd, struct timeval *tv);

void
countdown_timeval(unsigned long curr_clock, struct timeval *tv);

int
isChanged(int x, int y, int weight, int array[NUM][NUM]);


#endif
