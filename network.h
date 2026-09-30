// network.h

#ifndef NETWORK_H
#define NETWORK_H

#include <sys/socket.h>
#include <netinet/in.h>

typedef struct {
  int socket;
  int client;
  struct sockaddr_in addr;
  socklen_t addr_len;
} server_properties;

int create_socket(server_properties *s);

void setup_address(server_properties *s);

int bind_socket(server_properties *s);

int start_listen(server_properties *s);

int connect_client(server_properties *s);

#endif
