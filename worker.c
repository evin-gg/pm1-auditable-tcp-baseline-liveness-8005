#include "network.h"
#include <crypt.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
// #include <string.h>

int main(int argc, char *argv[]) {
  server_properties s = {0};
  in_port_t port;

  struct ClientArgs args = {0};


  if (parse_client_args(argc, argv, &args.host, &args.port_str, &args.worker,
                        &args.heartbeat) == 1) {
    return EXIT_SUCCESS;
  }

  if (parse_client_args(argc, argv, &args.host, &args.port_str, &args.worker,
                        &args.heartbeat) == -1) {
    return EXIT_FAILURE;
  }

  if (validate_client_args(args, &port, &s) == -1) {
    printf("ERROR: Failed to validate arguments\n");
    return EXIT_FAILURE;
      }

  if (setup_server_address(&s, &port) == -1) {
    printf("ERROR: Failed to initialize server struct\n");
    return EXIT_FAILURE;
  }

  if (create_socket(&s) == -1) {
    printf("ERROR: Failed to create socket\n");
    return EXIT_FAILURE;
  }

  if (connect_client(&s) == -1) {
    printf("ERROR: Couldn't connect to server\n");
    return EXIT_FAILURE;
  }

  char buf[6] = {0};
  recv(s.socket, buf, 6, 0);

  printf("%s\n", buf);

  printf("ending program\n");
  return EXIT_SUCCESS;
}
