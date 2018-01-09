#include "network.h"

void subcommittee(int from_client);

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove("luigi");
    exit(0);
  }
}

int main() {

  signal(SIGINT, sighandler);
  int listen_socket = committee_setup();

  while (1) {
    int client_socket = committee_connect(listen_socket);
    if (fork()) {
      printf("forked!\n");
      close(client_socket);
    } //parent
    else {//child
      subcommittee(client_socket);
    }
  }
}

void print_bout(struct bout tada) {
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

  while(read(client_socket, buffer, sizeof(buffer))) {
    char * input = strdup(buffer);
    char * type = strsep(&input, ":");
    printf("%s\n", type);
    if (strcmp(type, "ref") == 0) {
      this_bout.referee = input;
    }
    else if (strcmp(type, "win") == 0) {
      this_bout.winner = input;
    }
    else if (strcmp(type, "los") == 0) {
      this_bout.loser = input;
    }
    else if (strcmp(type, "wsc") == 0) {
      this_bout.win_score = atoi(input);
    }
    else if (strcmp(type, "lsc") == 0) {
      this_bout.lose_score = atoi(input);
      print_bout(this_bout);
      bout_array[bout_count] = this_bout;
      bout_count++;
    }
    else {
      printf("Something isn't right\n");
      exit(0);
    }
    write(client_socket, buffer, sizeof(buffer));
  }
  close(client_socket);
  exit(0);
}
