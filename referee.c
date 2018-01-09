#include "network.h"

/*=========================
prompt
args: char * message, int committee_socket, char * type

message is the prompt to be displayed for the user.
committee_socket is the socket descriptor of the server/bout committee
type is the type of input (e.g. referee name, winner name, winner score, etc...)

Displays the prompt, takes the appropriate input, sends it to the bout committee, and receives verification

=========================*/

void prompt(char * message, int committee_socket, char * type) {// modular design! Takes input and sends it to server.

  char buffer[BUFFER_SIZE];

  printf("%s", message); //prompt for input
  fgets(buffer, sizeof(buffer), stdin); //takes input
  *strchr(buffer, '\n') = 0;
  char * tmp = strdup(buffer);
  strcpy(buffer, type);
  strcat(buffer, tmp);//add type to beginning
  free(tmp); //concats to add the info type to the beginning (e.g. ref, win, los...)
  write(committee_socket, buffer, sizeof(buffer)); //writes to socket
  read(committee_socket, buffer, sizeof(buffer)); //reads received value from socket
  printf("received: [%s]\n", buffer);
}





int main(int argc, char **argv) {

  int committee_socket; //listening server stream socket
  char buffer[BUFFER_SIZE];

  if (argc == 2) //if there's an address specified in the run command
    committee_socket = client_setup(argv[1]); //connect socket to that address
  else
    committee_socket = client_setup(LOOPBACK); //if no address specified, connect to itself using loopback address

  while (1) {
    prompt("Referee name: \n", committee_socket, "ref:"); //prompt referee name
    prompt("Bout winner: \n", committee_socket, "win:"); //prompt winner name
    prompt("Bout loser: \n", committee_socket, "los:"); //prompt loser name
    prompt("Winner's score: \n", committee_socket, "wsc:"); //prompt winner score
    prompt("Loser's score: \n", committee_socket, "lsc:"); //prompt loser score
  }
}
