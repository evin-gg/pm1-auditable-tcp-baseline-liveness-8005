#include "network.h"
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  server_properties server = {0};
  in_port_t port;

  char *ip_str = argv[1];
  char *port_str = argv[2];

  // validate args
  if (validate_args(argc, argv, &server, &port) == -1) {
    printf("ERROR: Failed to validate arguments\n"); 
    return EXIT_FAILURE;
  }

  if (create_socket(&server) == -1) {
    printf("ERROR: Socket creation failed");
    return EXIT_FAILURE;
  }

  setup_server_address(&server, &port);

  if (bind_socket(&server)) {
    printf("ERROR: Failed to bind\n");
    return EXIT_FAILURE;
  }

  if (start_listen(&server) == -1) {
    printf("ERROR: Failed to start listening\n");
    return EXIT_FAILURE;
  }

  server.client =
      accept(server.socket, (struct sockaddr *)&server.addr, &server.addr_len);

  send(server.client, "COMMS\n", 6, 0);

  close(server.socket);
  printf("Ending Program\n");
  return EXIT_SUCCESS;
};
