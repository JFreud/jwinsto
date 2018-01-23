#include "network.h"

void error_check( int i, char *s ) {
  if ( i < 0 ) {
    printf("[%s] error %d: %s\n", s, errno, strerror(errno) );
    exit(1);
  }
}


int committee_setup() { //creates, binds listening socket
  int sd; //sd is socket descriptor,
  sd = socket(AF_INET, SOCK_STREAM, 0); //create stream socket with ipv4
  printf("sd: %d\n", sd);
  struct addrinfo * hints, * results;
  //hints: Pointer to a struct addrinfo used to provide settings for the lookup (type of address, etc.)
  //results: Pointer to a struct addrinfo that will be a linked list containing entries for each matching address
  hints = (struct addrinfo *) calloc(1, sizeof(struct addrinfo));
  hints -> ai_family = AF_INET; //ipV4
  hints -> ai_socktype = SOCK_STREAM; //TCP Stream Socket
  hints -> ai_flags = AI_PASSIVE; //Use all valid addresses
  getaddrinfo(NULL, PORT, hints, &results); //allocates memory for structs
  int yes = 1;
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (void*) &yes, (socklen_t) sizeof(yes));
  int i = bind(sd, results -> ai_addr, results -> ai_addrlen); //i is status of bind (0 or -1). Bind takes arguments socket descriptor, address, address length
  error_check( i, "server bind" );
  printf("[server] socket bound\n");
  //ai_addr: Pointer to a struct sockaddr containing the IP address.
  //ai_addrlen: Size of the address in bytes.
  i = listen(sd, 128); //set socket to passively wait connetion. Backlog allows 128 queued connections (standard number)
  error_check(i, "committee listen");
  free(hints); //frees allocated memory
  freeaddrinfo(results); //same here
  return sd; //returns the socket descriptor (number in file table)
}



int committee_connect(int sd) {//runs accept to connect committee with client
  int to_client; //descriptor to new socket
  socklen_t sock_size; //like size_t
  struct sockaddr_storage client_address; //
  sock_size = sizeof(client_address);
  int client_socket = accept(sd, (struct sockaddr *)&client_address, &sock_size); //accept next client, performs server side of handshake, creates new socket for communicating with client
  error_check(client_socket, "server accept");
  return client_socket; //returns new socket descriptor
}


int client_setup(char * committee) {//create and connect a socket to the listening server, char * committee is a string representing server/committee IP
  int sd, i; //socket descriptor
  sd = socket(AF_INET, SOCK_STREAM, 0); //create stream socket with ipv4
  error_check( sd, "client socket" );
  struct addrinfo * hints, * results;
  //hints: Pointer to a struct addrinfo used to provide settings for the lookup (type of address, etc.)
  //results: Pointer to a struct addrinfo that will be a linked list containing entries for each matching address
  hints = (struct addrinfo *) calloc(1, sizeof(struct addrinfo));
  hints -> ai_family = AF_INET; //ipV4
  hints -> ai_socktype = SOCK_STREAM; //TCP Stream Socket
  getaddrinfo(committee, PORT, hints, &results); //allocates memory for structs
  i = connect(sd, results -> ai_addr, results -> ai_addrlen); //connect to socket currently in listening state, client side handshake, binds to address/port, blocks until connection made
  error_check( i, "client connect" );
  free(hints); //free memory
  freeaddrinfo(results); //same here
  return sd; //returns the socket descriptor (number in file table)
}
