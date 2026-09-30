#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include "network.h"


int main(int argc, char *argv[])
{
  server_properties server;

  if(create_socket(&server) == -1) 
  {
    printf("ERROR: Socket creation failed");
    return EXIT_FAILURE;
  }

  setup_address(&server);

  if (bind_socket(&server))
  {
    printf("ERROR: Failed to bind\n");
    return EXIT_FAILURE;
  }

  if(start_listen(&server) == -1)
  {
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

// int create_socket(server_properties *s) {
//   s->socket = socket(AF_INET, SOCK_STREAM, 0);
//   if (s->socket == -1) {
//     return -1;
//   }
//
//   return 0;
// };
//
// void setup_address(server_properties *s) {
//   s->addr.sin_addr.s_addr = INADDR_ANY;
//   s->addr.sin_family = AF_INET;
//   s->addr.sin_port = htons(9001);
//   s->addr_len = sizeof(s->addr);
// };
//
// int bind_socket(server_properties *s)
// {
//   int status = 0;
//   if (bind(s->socket, (struct sockaddr *) &s->addr, s->addr_len) == -1)
//   {
//     return -1;
//   }
//
//   return 0;
// };
//
// int start_listen(server_properties *s)
// {
//   if(listen(s->socket, 1) == -1)
//   {
//     return -1;
//   }
//
//   return 0;
// }; 
