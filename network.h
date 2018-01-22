#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
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
#define KEY 666 //en garde!



struct bout {
  char * referee;
  char * winner;
  char * loser;
  int win_score;
  int lose_score;
};


struct referee {
  char * first_name;
  char * last_name;
  char * country;
  char * club;
};

struct fencer {
  char * first_name;
  char * last_name;
  char * country;
  char * club;
  char * rating;
};

struct pool_fencer {
  char * first_name;
  char * last_name;
  int victoires;
  int ts; //touches scored
  int tr; //touches received
  int ind; //indicator
  int plc; //place (in pool)
}

 // union semun {
 //   int              val;    /* Value for SETVAL */
 //   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
 //   unsigned short  *array;  /* Array for GETALL, SETALL */
 //   struct seminfo  *__buf;  /* Buffer for IPC_INFO                                      (Linux specific) */
 // };

//fxn headers:...



int committee_setup();
int committee_connect(int sd);
int client_setup(char * committee);
