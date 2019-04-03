#ifndef HELPER_H
#define HELPER_H

// Helper functions for main program

#include <sys/socket.h>
#include <ctime>
#include <vector>

#define TIMEOUT_SEC 6
#define TIMEOUT_USEC 500000

void *
get_in_addr(struct sockaddr *sa);

unsigned short int
get_in_port(struct sockaddr *sa);

bool
compare_sockaddr(struct sockaddr *sa_a, struct sockaddr *sa_b);

bool
add_new_addr(struct sockaddr *sa, std::vector<struct sockaddr *> &sa_list);

int
send_all(int send_fd, const char *buf, int *len, struct sockaddr *to, socklen_t to_len);

int
setup_socket_udp(const char *port);

struct addrinfo *
get_address(const char *addr, const char *port);

bool
wait_for_packet(int router_fd, struct timeval *tv);

void
countdown_timeval(std::clock_t curr_clock, struct timeval *tv);

#endif
