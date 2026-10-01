#include "network.h"
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define HASH_SETTING "$6$rounds=5000$comp8005$"

int main(int argc, char *argv[]) {

  server_properties server = {0};
  in_port_t port;

  struct ServerArgs args = {0};
  server.protocol = AF_INET;

  if (parse_server_args(argc, argv, &args.port_str, &args.password,
                        &args.charset_file, &args.timeout_ms) == 1) {
    return EXIT_SUCCESS;
  }


  if (parse_server_args(argc, argv, &args.port_str, &args.password,
                        &args.charset_file, &args.timeout_ms) == -1) {
    return EXIT_FAILURE;
  }

  // validate args
  if (validate_server_args(args, &port) == -1) {
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
  printf("Listening on %s\n", args.port_str);

  server.client =
      accept(server.socket, (struct sockaddr *)&server.addr, &server.addr_len);

  send(server.client, "COMMS\n", 6, 0);

  close(server.socket);
  printf("Ending Program\n");
  return EXIT_SUCCESS;
};
