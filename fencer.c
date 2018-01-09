#include "network.h"


//copy paste from pipe_networking
int main(int argc, char **argv) {

  int committee_socket;
  char buffer[BUFFER_SIZE];

  if (argc == 2)
    committee_socket = client_setup(argv[1]);
  else
    committee_socket = client_setup(LOOPBACK);

  while (1) {
    printf("enter complaint: ");
    fgets(buffer, sizeof(buffer), stdin);
    *strchr(buffer, '\n') = 0;
    write(committee_socket, buffer, sizeof(buffer));
    read(committee_socket, buffer, sizeof(buffer));
    printf("received: [%s]\n", buffer);
  }
}
