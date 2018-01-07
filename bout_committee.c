#include "network.h"

void process(char *s);
void subcommittee(int from_client);

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove("luigi");
    exit(0);
  }
}

int main() {
  while (1) {
    signal(SIGINT, sighandler);
    int from_client;
    from_client = committee_setup();
    if(from_client){
      int f = fork();
      if (f) {
        printf("forked!\n");
      } //parent
      else {//child
        subcommittee(from_client);
      }
    }
  }
}

void print_bout(struct bout tada) {
  printf("Ref: %s\n", tada.referee);
  printf("Winner: %s\n", tada.winner);
  printf("Loser: %s\n", tada.loser);
  printf("win_score: %d\n", *tada.win_score);
  printf("lose_score: %d\n", *tada.lose_score);
}

void subcommittee(int from_client) {
  int to_client = committee_connect(from_client);// handshake should be finished
  char buffer[BUFFER_SIZE];

  struct bout this_bout;

  while(1) {
    read(from_client, this_bout.referee, sizeof(this_bout.referee));
    write(to_client, this_bout.referee, sizeof(this_bout.referee));

    read(from_client, this_bout.winner, sizeof(this_bout.winner));
    write(to_client, this_bout.winner, sizeof(this_bout.winner));

    read(from_client, this_bout.loser, sizeof(this_bout.loser));
    write(to_client, this_bout.loser, sizeof(this_bout.loser));

    read(from_client, this_bout.win_score, sizeof(this_bout.win_score));
    write(to_client, this_bout.win_score, sizeof(this_bout.win_score));

    read(from_client, this_bout.lose_score, sizeof(this_bout.lose_score));
    write(to_client, this_bout.lose_score, sizeof(this_bout.lose_score));
    // process(buffer);
    // printf("[committee] performed process: \n");
    print_bout(this_bout);
    write(to_client, buffer, sizeof(buffer));
  }
  exit(0);
}

void process(char * s) {
  while (*s) { //make caps
    *s = toupper(*s);
    s++;
  }
}
