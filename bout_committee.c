#include "network.h"

void process(char *s);
void subserver(int from_client);

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
    from_client = server_setup();
    if(from_client){
      int f = fork();
      if (f) {
        printf("forked!\n");
      } //parent
      else {//child
        subserver(from_client);
      }
    }
  }
}

void subserver(int from_client) {
  int to_client = server_connect(from_client);// handshake should be finished
  char buffer[BUFFER_SIZE];

  while(read(from_client, buffer, sizeof(buffer))) {
    process(buffer);
    printf("[server] performed process: %s\n", buffer);
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
