#include "network.h"



int committee_setup() {
  int sd, i;
  sd = socket(AF_INET, SOCK_STREAM, 0);
  struct addrinfo * hints, * results;
  hints = calloc(1, sizeof(struct addrinfo));
  hints -> ai_family = AF_INET;
  hints -> ai_socktype = SOCK_STREAM;
  hints -> ai_flags = AI_PASSIVE;
  getaddrinfo(NULL, PORT, hints, &results);
  i = bind(sd, results -> ai_addr, results -> ai_addrlen);
  i = listen(sd, 128);
  free(hints);
  freeaddrinfo(results);
  return sd;
}



int committee_connect(int sd) {
  int to_client;
  socklen_t sock_size;
  struct sockaddr_storage client_socket;
  to_client = accept(sd, (struct sockaddr *)&client_socket, &sock_size);
  return to_client;
}


int client_setup(char * committee) {
  int sd, i;
  sd = socket(AF_INET, SOCK_STREAM, 0);
  struct addrinfo * hints, * results;
  hints = calloc(1, sizeof(struct addrinfo));
  hints -> ai_family = AF_INET;
  hints -> ai_socktype = SOCK_STREAM;
  getaddrinfo(committee, PORT, hints, &results);
  i = connect(sd, results -> ai_addr, results -> ai_addrlen);
  free(hints);
  freeaddrinfo(results);
  return sd;
}
