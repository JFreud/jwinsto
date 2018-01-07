#include "network.h"

/*=========================
  committee_setup
  args:
  creates the WKP (upstream) and opens it, waiting for a
  connection.
  removes the WKP once a connection has been made
  returns the file descriptor for the upstream pipe.
  =========================*/
int committee_setup() {
  int from_referee;
  mkfifo("luigi", 0600);
  printf("[committee] handshake: making wkp\n");
  from_referee = open( "luigi", O_RDONLY, 0);
  // read(from_referee, buffer, sizeof(buffer));
  remove("luigi");
  printf("[committee] handshake: removed wkp\n");
  return from_referee;
}


/*=========================
  committee_connect
  args: int from_referee
  handles the subcommittee portion of the 3 way handshake
  returns the file descriptor for the downstream pipe.
  =========================*/
int committee_connect(int from_referee) {
  char buffer[HANDSHAKE_BUFFER_SIZE];
  read(from_referee, buffer, sizeof(buffer));
  printf("[committee] handshake: received [%s]\n", buffer);

  int to_referee;
  to_referee = open(buffer, O_WRONLY, 0);
  write(to_referee, buffer, sizeof(buffer));

  read(from_referee, buffer, sizeof(buffer));
  printf("[committee] handshake received: %s\n", buffer);

  return to_referee;
}

/*=========================
  committee_handshake
  args: int * to_referee
  Performs the committee side pipe 3 way handshake.
  Sets *to_referee to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int committee_handshake(int *to_referee) {

  int from_referee;

  char buffer[HANDSHAKE_BUFFER_SIZE];

  mkfifo("luigi", 0600);

  //block on open, recieve mesage
  printf("[committee] handshake: making wkp\n");
  from_referee = open( "luigi", O_RDONLY, 0);
  read(from_referee, buffer, sizeof(buffer));
  printf("[committee] handshake: received [%s]\n", buffer);

  remove("luigi");
  printf("[committee] handshake: removed wkp\n");

  //connect to referee, send message
  *to_referee = open(buffer, O_WRONLY, 0);
  write(*to_referee, buffer, sizeof(buffer));

  //read for referee
  read(from_referee, buffer, sizeof(buffer));
  printf("[committee] handshake received: %s\n", buffer);

  return from_referee;
}

/*=========================
  referee_handshake
  args: int * to_committee
  Performs the referee side pipe 3 way handshake.
  Sets *to_committee to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
int referee_handshake(int *to_committee) {

  int from_committee;
  char buffer[HANDSHAKE_BUFFER_SIZE];

  //send pp name to committee
  printf("[referee] handshake: connecting to wkp\n");
  *to_committee = open( "luigi", O_WRONLY, 0);
  if ( *to_committee == -1 )
    exit(1);

  //make private pipe
  sprintf(buffer, "%d", getpid() );
  mkfifo(buffer, 0600);

  write(*to_committee, buffer, sizeof(buffer));

  //open and wait for connection
  from_committee = open(buffer, O_RDONLY, 0);
  read(from_committee, buffer, sizeof(buffer));
  /*validate buffer code goes here */
  printf("[referee] handshake: received [%s]\n", buffer);

  //remove pp
  remove(buffer);
  printf("[referee] handshake: removed pp\n");

  //send ACK to committee
  write(*to_committee, REF, sizeof(buffer));

  return from_committee;
}
