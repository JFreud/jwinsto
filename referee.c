#include "network.h"



void prompt(char * message, int committee_socket, char * type) {

  char buffer[BUFFER_SIZE];

  printf("%s", message);
  fgets(buffer, sizeof(buffer), stdin);
  *strchr(buffer, '\n') = 0;
  char * tmp = strdup(buffer);
  strcpy(buffer, type);
  strcat(buffer, tmp);//add type to beginning
  free(tmp);
  write(committee_socket, buffer, sizeof(buffer));
  read(committee_socket, buffer, sizeof(buffer));
  printf("received: [%s]\n", buffer);
}





int main(int argc, char **argv) {

  int committee_socket;
  char buffer[BUFFER_SIZE];

  if (argc == 2)
    committee_socket = client_setup(argv[1]);
  else
    committee_socket = client_setup(LOOPBACK);

  while (1) {
    prompt("Referee name: \n", committee_socket, "ref:");
    prompt("Bout winner: \n", committee_socket, "win:");
    prompt("Bout loser: \n", committee_socket, "los:");
    prompt("Winner's score: \n", committee_socket, "wsc:");
    prompt("Loser's score: \n", committee_socket, "lsc:");
  }
}
