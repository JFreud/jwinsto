#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>
#include <time.h>

#define FEN "OPA"
#define REF "BLACK_CARD"
#define HANDSHAKE_BUFFER_SIZE 10
#define BUFFER_SIZE 1000




struct bout {
  int tableau;
  int winner_seed;
  int loser_seed;
  char referee[128];
  char winner[128];
  char loser[128];
  int * win_score; //because read() takes void *
  int * lose_score; //because read() takes void *
};

//fxn headers:...


int committee_handshake(int *to_client);
int referee_handshake(int *to_committee);
int fencer_handshake(int *to_committee);




int committee_setup();
int committee_connect(int from_client);
