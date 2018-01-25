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
  struct sembuf semtake = {.sem_num = 0, .sem_op = -1, .sem_flg = SEM_UNDO}; //init struct for taking spot
  struct sembuf semrelease = {.sem_num = 0, .sem_op = 1, .sem_flg = SEM_UNDO}; //init struct for giving up spot
  int retval; //semop return value
  int semid = semget(KEY, 1, 0); //get semaphore
  semop(semid, &semtake, 1); //decrease semaphore before accessing resources

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
  if (strcmp(buffer, "finished") == 0) {
    exit(0);
  }

  semop(semid, &semrelease, 1); //increase semaphore when finished
}

void print_bout(struct bout tada) { //prints the bout struct
  printf("Ref: %s\n", tada.referee);
  printf("Winner: %s\n", tada.winner);
  printf("Loser: %s\n", tada.loser);
  printf("win_score: %d\n", tada.win_score);
  printf("lose_score: %d\n", tada.lose_score);
}

static void sighandler(int signo) {
  if (signo == 0) {
    // close(committee_socket);
    printf("Bam blam\n");
    exit(1);
  }
}


int main(int argc, char **argv) {

  // int committee_socket; //listening server stream socket
  char buffer[BUFFER_SIZE];
  int committee_socket;

  union semun argument;
  signal(SIGINT, sighandler);

  if (argc == 2) //if there's an address specified in the run command
    committee_socket = client_setup(argv[1]); //connect socket to that address
  else
    committee_socket = client_setup(LOOPBACK); //if no address specified, connect to itself using loopback address

  int semid = semget(KEY, 1, IPC_CREAT | 0666); //create semaphore
  if (semid < 0) { //if it returned -1
    printf("%s\n", strerror(errno)); //print error
    exit(1); //quit
  }
  argument.val = 1;
  if (semctl(semid, 0, SETVAL, argument) < 0) { //set value of semaphore 0 with semid to 1
    printf("Error: %s\n", strerror(errno)); //if it returned -1
    exit(1); //quit
  }


  //check what stage the tournament is in
  printf("What stage of the tournament are you in?\nChoose: [1] for pools\t[2] for DEs.\n");
  fgets(buffer, sizeof(buffer), stdin); //takes input
  *strchr(buffer, '\n') = 0;
  int stage = atoi(buffer) - 1;

  if (!stage) { //pools was chosen
    prompt("Last Name?: \n", committee_socket, "ref:"); //prompt referee name
    printf("Registered Ref\n");
    // // read(committee_socket, buffer, sizeof(buffer)); //reads received value from socket
    // printf("afawf");
    // printf("%s\n", buffer);


    while (1) {

      prompt("Bout winner: \n", committee_socket, "win:"); //prompt winner name
      prompt("Winner's score: \n", committee_socket, "wsc:"); //prompt winner score
      prompt("Bout loser: \n", committee_socket, "los:"); //prompt loser name
      prompt("Loser's score: \n", committee_socket, "lsc:"); //prompt loser score
    }
  }
  else { //DEs was chosen

    prompt("Last Name?: \n", committee_socket, "ref:"); //prompt referee name
    printf("Registered Ref\n");


    while (1) {

      prompt("Bout winner: \n", committee_socket, "win:"); //prompt winner name
      prompt("Winner's score: \n", committee_socket, "wsc:"); //prompt winner score
      prompt("Bout loser: \n", committee_socket, "los:"); //prompt loser name
      prompt("Loser's score: \n", committee_socket, "lsc:"); //prompt loser score
    }

  }


}
