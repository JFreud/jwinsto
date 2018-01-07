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
  printf("win_score: %d\n", tada.win_score);
  printf("lose_score: %d\n", tada.lose_score);
}

void subcommittee(int from_client) {
  int to_client = committee_connect(from_client);// handshake should be finished
  char buffer[BUFFER_SIZE];

  struct bout this_bout;
  struct bout bout_array[1000];
  int bout_count = 0;

  while(read(from_client, buffer, sizeof(buffer))) {
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
