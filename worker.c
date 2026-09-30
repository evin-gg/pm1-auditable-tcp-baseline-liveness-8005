#include "network.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  server_properties s;

  if (create_socket(&s) == -1) {
    printf("ERROR: Failed to create socket\n");
    return EXIT_FAILURE;
  }


  printf("ending program");
  return EXIT_SUCCESS;
}
