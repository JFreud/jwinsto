#include "network.h"

/*=========================
  committee_setup
  args:
  creates the WKP (upstream) and opens it, waiting for a
  connection.
  removes the WKP once a connection has been made
  returns the file descriptor for the upstream pipe.
  =========================*/
// int committee_setup() {
//   int from_referee;
//   mkfifo("luigi", 0600);
//   printf("[committee] handshake: making wkp\n");
//   from_referee = open( "luigi", O_RDONLY, 0);
//   // read(from_referee, buffer, sizeof(buffer));
//   remove("luigi");
//   printf("[committee] handshake: removed wkp\n");
//   return from_referee;
// }

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




// /*=========================
//   committee_connect
//   args: int from_referee
//   handles the subcommittee portion of the 3 way handshake
//   returns the file descriptor for the downstream pipe.
//   =========================*/
// int committee_connect(int from_referee) {
//   char buffer[HANDSHAKE_BUFFER_SIZE];
//   read(from_referee, buffer, sizeof(buffer));
//   printf("[committee] handshake: received [%s]\n", buffer);
//
//   int to_referee;
//   to_referee = open(buffer, O_WRONLY, 0);
//   write(to_referee, buffer, sizeof(buffer));
//
//   read(from_referee, buffer, sizeof(buffer));
//   printf("[committee] handshake received: %s\n", buffer);
//
//   return to_referee;
// }

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
