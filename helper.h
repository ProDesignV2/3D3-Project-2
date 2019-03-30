#ifndef HELPER_H
#define HELPER_H

// Helper functions for main program

#include <sys/socket.h>

void *
get_in_addr(struct sockaddr *sa);

unsigned short int
get_in_port(struct sockaddr *sa);

int
send_all(int send_fd, const char *buf, int *len, struct sockaddr *to, socklen_t to_len);

int
setup_socket_udp(const char *port);

struct addrinfo *
get_address(const char *addr, const char *port);

#endif
