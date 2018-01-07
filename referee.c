#include "network.h"



void prompt(char * message, int to_committee, int from_committee, char * type) {

  char buffer[BUFFER_SIZE];

  printf("%s", message);
  fgets(buffer, sizeof(buffer), stdin);
  *strchr(buffer, '\n') = 0;
  char * tmp = strdup(buffer);
  strcpy(buffer, type);
  strcat(buffer, tmp);//add type to beginning
  free(tmp);
  write(to_committee, buffer, sizeof(buffer));
  read(from_committee, buffer, sizeof(buffer));
  printf("received: [%s]\n", buffer);
}




//copy paste from pipe_networking
int main() {

  int to_committee;
  int from_committee;
  char buffer[BUFFER_SIZE];
  from_committee = referee_handshake( &to_committee );

  while (1) {
    prompt("Referee name: \n", to_committee, from_committee, "ref:");
    prompt("Bout winner: \n", to_committee, from_committee, "win:");
    prompt("Bout loser: \n", to_committee, from_committee, "los:");
    prompt("Winner's score: \n", to_committee, from_committee, "wsc:");
    prompt("Loser's score: \n", to_committee, from_committee, "lsc:");
  }
}
