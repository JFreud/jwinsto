#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

#define FEN "OPA"
#define REF "BLACK_CARD"
#define HANDSHAKE_BUFFER_SIZE 10
#define BUFFER_SIZE 1000
#define PORT "9001"
#define LOOPBACK "127.0.0.1"



struct bout {
  int tableau;
  int winner_seed;
  int loser_seed;
  char * referee;
  char * winner;
  char * loser;
  int win_score;
  int lose_score;
};

//fxn headers:...



int committee_setup();
int committee_connect(int sd);
int client_setup(char * committee);
