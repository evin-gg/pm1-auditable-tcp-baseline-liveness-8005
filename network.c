#include "network.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int create_socket(server_properties *s) {
  if (s->protocol == AF_INET) {
    s->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (s->socket == -1) {
      return -1;
    }
    printf("IPV4 SOCKET\n");
  }

  else {
    s->socket = socket(AF_INET6, SOCK_STREAM, 0);
    if (s->socket == -1) {
      return -1;
    }

    printf("IPV6 SOCKET\n");
  }

  return 0;
};

int setup_server_address(server_properties *s, in_port_t *port) {

  if (s->protocol == AF_INET) {
    s->addr.sin_port = htons(*port);
    s->addr_len = sizeof(struct sockaddr_in);
    printf("ITS IPV4\n");
  }

  else {
    s->addr6.sin6_port = htons(*port);
    s->addr_len = sizeof(struct sockaddr_in6);
    printf("its IPV6\n");
  }

  return 0;
};

int bind_socket(server_properties *s) {
  int status = 0;

  if (s->protocol == AF_INET) {
    if (bind(s->socket, (struct sockaddr *)&s->addr, s->addr_len) == -1) {
      perror("Binding Failed");
      return -1;
    }
    printf("binding ipv4\n");
  }

  else {
    printf("binding ipv6\n");
    if (bind(s->socket, (struct sockaddr *)&s->addr6, s->addr_len) == -1) {
      perror("Binding Failed");
      return -1;
    }
  }

  printf("binded\n");
  return 0;
};

int start_listen(server_properties *s) {
  if (listen(s->socket, 1) == -1) {
    return -1;
  }

  printf("listening\n");
  return 0;
};

int connect_client(server_properties *s) {
  if (s->protocol = AF_INET) {
    int status = connect(s->socket, (struct sockaddr *)&s->addr, s->addr_len);
    return status;
  }

  else {
    int status = connect(s->socket, (struct sockaddr *)&s->addr6, s->addr_len);
    return status;
  }
}

int valid_ip(const char *ip, server_properties *s) {
  struct in_addr ipv4;
  struct in6_addr ipv6;

  if (inet_pton(AF_INET, ip, &ipv4) == 1) {
    s->addr.sin_family = AF_INET;
    s->addr.sin_addr = ipv4;
    printf("IPV4 Detected\n");
    s->protocol = AF_INET;
    return 0;
  }

  if (inet_pton(AF_INET6, ip, &ipv6) == 1) {
    s->addr6.sin6_family = AF_INET6;
    s->addr6.sin6_addr = ipv6;
    printf("IPV6 Detected\n");
    s->protocol = AF_INET6;
    return 0;
  }

  return -1;
}

int parse_port(char *portstr, in_port_t *p) {

  // check for null
  if (portstr == NULL) {
    printf("ERROR: Port is null\n");
    return -1;
  }

  char *endptr;
  long longport = strtol(portstr, &endptr, 10);

  // invalid chars
  if (endptr == portstr || *endptr != '\0') {
    printf("ERROR: Invalid port\n");
    return -1;
  }

  // range
  if (longport < 1 || longport > 65535) {
    printf("ERROR: Port out of range\n");
    return -1;
  }

  *p = (in_port_t)longport;
  return 0;
}

void usage() { printf("USAGE: [IP Address] [Port]\n"); }

int validate_args(int argc, char *args[], server_properties *s, in_port_t *p) {

  // argument count
  if (argc != 3) {
    usage();
    return -1;
  }

  // validate IP
  if (valid_ip(args[1], s) == -1) {
    printf("ERROR: Invalid IP Address\n");
    usage();
    return -1;
  }

  // validate port
  if (parse_port(args[2], p) == -1) {
    usage();
    return -1;
  }

  return 0;
}
