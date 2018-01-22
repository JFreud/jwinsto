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

  semop(semid, &semrelease, 1); //increase semaphore when finished
}





int main(int argc, char **argv) {

  int committee_socket; //listening server stream socket
  char buffer[BUFFER_SIZE];

  union semun argument;

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


    /***
    struct bout {
  int tableau;
  int winner_seed;
  int loser_seed;
  char * referee;
  char * winner;
  char * loser;
  int win_score;
  int lose_score;
};  ***/

    while (1){
        
        char buffer[BUFFER_SIZE];
        struct bout new_bout;
        
        printf("Referee Last Name? \n");
        fgets(buffer, sizeof(buffer), stdin); //takes input
        new_bout.referee = buffer;
        
        printf("Winner Last Name? \n");
        fgets(buffer, sizeof(buffer), stdin); //takes input
        new_bout.winner = buffer;
        
        printf("Winner Score? \n");
        fgets(buffer, sizeof(buffer), stdin);
        new_bout.win_score = atoi(buffer);
        
        printf("Loser Last Name?: \n");
        fgets(buffer, sizeof(buffer), stdin); //takes input
        new_bout.loser = buffer;
        
        printf("Loser Score? \n");
        fgets(buffer, sizeof(buffer), stdin);
        new_bout.lose_score = atoi(buffer);
        
        write(committee_socket, &new_bout, sizeof(new_bout));
        
    }
    
    
    /***
  while (1) {
    prompt("Last Name?: \n", committee_socket, "ref:"); //prompt referee name
    prompt("Bout winner: \n", committee_socket, "win:"); //prompt winner name
    prompt("Bout loser: \n", committee_socket, "los:"); //prompt loser name
    prompt("Winner's score: \n", committee_socket, "wsc:"); //prompt winner score
    prompt("Loser's score: \n", committee_socket, "lsc:"); //prompt loser score
  }***/
}
