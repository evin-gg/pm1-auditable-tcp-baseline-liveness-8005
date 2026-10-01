// network.h

#ifndef NETWORK_H
#define NETWORK_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>

typedef struct {
  int socket;
  int client;
  int protocol;
  struct sockaddr_in addr;
  struct sockaddr_in6 addr6;
  socklen_t addr_len;
} server_properties;

int create_socket(server_properties *s);

int setup_server_address(server_properties *s, in_port_t *port);

int bind_socket(server_properties *s);

int start_listen(server_properties *s);

int connect_client(server_properties *s);

int valid_ip(const char *ip, server_properties *s);

int parse_port(char *portstr, in_port_t *p);

void usage();

int validate_args(int argc, char *args[], server_properties *s, in_port_t *p);

#endif
