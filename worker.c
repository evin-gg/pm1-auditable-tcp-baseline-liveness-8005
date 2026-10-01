#include "network.h"
#include <netinet/in.h>
#include <setjmp.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  server_properties s = {0};
    in_port_t port;

  if (validate_args(argc, argv, &s, &port) == -1) {
    printf("ERROR: Failed to validate arguments\n"); 
    return EXIT_FAILURE;
  }

  if(setup_server_address(&s, &port) == -1)
  {
      printf("ERROR: Failed to initialize server struct\n");
      return EXIT_FAILURE;
  }

  if (create_socket(&s) == -1) {
    printf("ERROR: Failed to create socket\n");
    return EXIT_FAILURE;
  }

  if(connect_client(&s) == -1) {
      printf("ERROR: Couldn't connect to server\n");
      perror("RIP");
      return EXIT_FAILURE;
  }

  char buf[6] = {0};
  recv(s.socket, buf, 6, 0);



  printf("ending program");
  return EXIT_SUCCESS;
}
