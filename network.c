#include "network.h"

int create_socket(server_properties *s) {
  s->socket = socket(AF_INET, SOCK_STREAM, 0);
  if (s->socket == -1) {
    return -1;
  }

  return 0;
};

void setup_address(server_properties *s) {
  s->addr.sin_addr.s_addr = INADDR_ANY;
  s->addr.sin_family = AF_INET;
  s->addr.sin_port = htons(9001);
  s->addr_len = sizeof(s->addr);
};

int bind_socket(server_properties *s)
{
  int status = 0;
  if (bind(s->socket, (struct sockaddr *) &s->addr, s->addr_len) == -1)
  {
    return -1;
  }

  return 0;
};

int start_listen(server_properties *s)
{
  if(listen(s->socket, 1) == -1)
  {
    return -1;
  }

  return 0;
}; 
