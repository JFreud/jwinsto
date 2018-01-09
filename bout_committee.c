#include "network.h"

void subcommittee(int from_client);

// static void sighandler(int signo) {
//   if (signo == SIGINT) {// if connection is interrupted with ctrl + c
//     remove("luigi");
//     exit(0);
//   }
// }

int main() {

  // signal(SIGINT, sighandler);
  int listen_socket = committee_setup(); //creates listening socket

  while (1) {
    int client_socket = committee_connect(listen_socket); //runs accept call to connect committee with client
    if (fork()) { //forking server!
      printf("forked!\n");
      close(client_socket); //end connection
    } //parent
    else {//child
      subcommittee(client_socket); //the forked child will deal with the client
    }
  }
}

void print_bout(struct bout tada) { //prints the bout struct
  printf("Ref: %s\n", tada.referee);
  printf("Winner: %s\n", tada.winner);
  printf("Loser: %s\n", tada.loser);
  printf("win_score: %d\n", tada.win_score);
  printf("lose_score: %d\n", tada.lose_score);
}

void subcommittee(int client_socket) {
  char buffer[BUFFER_SIZE];

  struct bout this_bout;
  struct bout bout_array[1000];//is this the number of bouts?
  int bout_count = 0;

  while(read(client_socket, buffer, sizeof(buffer))) { //read from client stream
    char * input = strdup(buffer);
    char * type = strsep(&input, ":");
    printf("%s\n", type);
    if (strcmp(type, "ref") == 0) { //if input is ref name fill out that part of bout info
      this_bout.referee = input;
    }
    else if (strcmp(type, "win") == 0) { //if input is winner name fill out that part of bout info
      this_bout.winner = input;
    }
    else if (strcmp(type, "los") == 0) { //if input is loser name fill out that part of bout info
      this_bout.loser = input;
    }
    else if (strcmp(type, "wsc") == 0) { //if input is winner score fill out that part of bout info
      this_bout.win_score = atoi(input);
    }
    else if (strcmp(type, "lsc") == 0) { //if input is loser score fill out that part of bout info
      this_bout.lose_score = atoi(input);
      print_bout(this_bout);
      bout_array[bout_count] = this_bout;
      bout_count++;
    }
    else { //incorrect input
      printf("Something isn't right\n");
      exit(0);
    }
    write(client_socket, buffer, sizeof(buffer)); //tell client what was received so it can print and user can verify
  }
  close(client_socket);
  exit(0);
}
