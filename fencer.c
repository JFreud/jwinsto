#include "network.h"


//copy paste from pipe_networking
int main() {

  int to_committee;
  int from_committee;
  char buffer[BUFFER_SIZE];

  from_committee = fencer_handshake( &to_committee );

  while (1) {
    printf("enter complaint: ");
    fgets(buffer, sizeof(buffer), stdin);
    *strchr(buffer, '\n') = 0;
    write(to_committee, buffer, sizeof(buffer));
    read(from_committee, buffer, sizeof(buffer));
    printf("received: [%s]\n", buffer);
  }
}
