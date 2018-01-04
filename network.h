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

#define ACK "OPA"
#define HANDSHAKE_BUFFER_SIZE 10
#define BUFFER_SIZE 1000


//fxn headers:...


int committee_handshake(int *to_client);
int referee_handshake(int *to_server);
int fencer_handshake(int (to_server));




int committee_setup();
int committee_connect(int from_client);
