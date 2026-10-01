#include "network.h"
#include <arpa/inet.h>
#include <ctype.h>
#include <getopt.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static struct option server_long_options[] = {
    {"port", required_argument, 0, 'p'},
    {"password", required_argument, 0, 'w'},
    {"charset-file", required_argument, 0, 'c'},
    {"timeout-ms", required_argument, 0, 't'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}};

static struct option client_long_options[] = {
    {"host", required_argument, 0, 'o'},
    {"port", required_argument, 0, 'p'},
    {"id", required_argument, 0, 'i'},
    {"heartbeat-ms", required_argument, 0, 't'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}};

int parse_client_args(int argc, char *argv[], char **o, char **p, char **i,
                      char **t) {
  int opt;
  while ((opt = getopt_long(argc, argv, "o:p:i:t:h", client_long_options,
                            NULL)) != -1) {

    switch (opt) {
    case 'o':
      printf("host = %s\n", optarg);
      *o = optarg;
      break;

    case 'p':
      printf("port = %s\n", optarg);
      *p = optarg;
      break;

    case 'i':
      printf("id = %s\n", optarg);
      *i = optarg;
      break;

    case 't':
      printf("heartbeat-ms = %s\n", optarg);
      *t = optarg;
      break;

    case 'h':
      printf("Usage: ./controller --host <host> --port <port> "
             "--id <worker-id> --heartbeat-ms <ms>\n");
      return 1;

    default:
      return -1;
    }
  }

  return 0;
}

int parse_server_args(int argc, char *argv[], char **p, char **w, char **c,
                      char **t) {
  int opt;
  while ((opt = getopt_long(argc, argv, "p:w:c:t:h", server_long_options,
                            NULL)) != -1) {

    switch (opt) {
    case 'p':
      printf("port = %s\n", optarg);
      *p = optarg;
      break;

    case 'w':
      printf("password = %s\n", optarg);
      *w = optarg;
      break;

    case 'c':
      printf("charset = %s\n", optarg);
      *c = optarg;
      break;

    case 't':
      printf("timeout = %s\n", optarg);
      *t = optarg;
      break;

    case 'h':
      printf("Usage: ./controller --port <port> --password <password> "
             "--charset-file <path> --timeout-ms <ms>\n");
      return 1;

    default:
      return -1;
    }
  }

  return 0;
}

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
    s->addr.sin_addr.s_addr = INADDR_ANY;
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
  if (s->protocol == AF_INET) {
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

int numcheck(char *nums) {
  for (int i = 0; i < strlen(nums); i++) {
    if (!isdigit(nums[i])) {
      return -1;
    }
  }

  return 0;
}

int validate_server_args(struct ServerArgs args, in_port_t *p) {

  // validate port
  if (parse_port(args.port_str, p) == -1) {
    return -1;
  }

  // pass
  if (strlen(args.password) > 2) {
    printf("ERROR: Password cannot exceed 2 characters\n");
  }

  // charset_file
  if (access(args.charset_file, F_OK) == 1) {
    printf("ERROR: Charset File path does not exist\n");
    return -1;
  }

  // timeout
  if (numcheck(args.timeout_ms) == -1) {
    printf("ERROR: Invalid timeout value");
    return -1;
  }

  return 0;
}

int validate_client_args(struct ClientArgs args, in_port_t *p,
                         server_properties *s) {
  // host
  if (valid_ip(args.host, s) == -1) {
    printf("ERROR: Invalid IP address\n");
    return -1;
  }
  // port
  if (parse_port(args.port_str, p) == -1) {
    return -1;
  }

  // id
  if (numcheck(args.worker) == -1) {
    printf("ERROR: Invalid worker-id value");
    return -1;
  }

  // heartbeat
  if (numcheck(args.heartbeat) == -1) {
    printf("ERROR: Invalid heartbeat value");
    return -1;
  }

  return 0;
}
