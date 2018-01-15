#include "network.h"

void subcommittee(int from_client);
struct referee * referee_list();
struct fencer * fencer_list();

// static void sighandler(int signo) {
//   if (signo == SIGINT) {// if connection is interrupted with ctrl + c
//     remove("luigi");
//     exit(0);
//   }
// }

void print_refs(struct referee * rlist) {
  while (rlist->last_name != NULL) {
    printf("fname: %s\n", rlist->first_name);
    printf("lname: %s\n", rlist->last_name);
    printf("country: %s\n", rlist->country);
    printf("club: %s\n", rlist->club);
    rlist++;
  }
}

void print_fens(struct fencer * fclist) {
  while (fclist->last_name != NULL) { //iterates through fencers by checking if last name exists
    printf("\nfname: %s\n", fclist->first_name);
    printf("lname: %s\n", fclist->last_name);
    printf("country: %s\n", fclist->country);
    printf("club: %s\n", fclist->club);
    printf("rating: %s\n", fclist->rating);
    fclist++;
  }
}

int count_fencers(struct fencer * fclist) {
  int count = 0;
  while (fclist->last_name != NULL) {
    fclist++;
    count++;
  }
  return count;
}

int count_referees(struct referee * rlist) {
  int count = 0;
    while (rlist->last_name != NULL) {
      rlist++;
      count++;
    }
    return count;
}

int comp_fc(const void * a, const void * b) { //comparison operator for qsort below
  struct fencer * fencerA = (struct fencer *)a;
  struct fencer * fencerB = (struct fencer *)b;
  printf("\nA: %c\n", fencerA->rating[0]);
  printf("B: %c\n", fencerB->rating[0]);
  printf("A v B: %d\n", fencerA->rating[0] - fencerB->rating[0]);
  int count = 1;
  if (fencerA->rating[0] - fencerB->rating[0] == 0) { // if the same letter rating
    printf("ASAME: %c\n", fencerA->rating[2]);
    printf("BSAME: %c\n", fencerB->rating[2]);
    while (fencerA->rating[count] - fencerB->rating[count] == 0 && count <= 2) { //keep increasing the index until a character is different
      count++; //should only take two iterations because only two digits of year rating was earned are present
    }
    if (!strcmp(&fencerA->rating[count], &fencerB->rating[count])) { //while loop finished and still equal
      return 0;
    }
    printf("%s %s %d\n", &fencerA->rating[count], &fencerB->rating[count], strcmp(&fencerA->rating[count], &fencerB->rating[count]));
    if (strcmp(&fencerA->rating[count], &fencerB->rating[count]) < 0) {
      return 1;
    } //multiply by -1 because higher numbers are better
    else {
      return -1;
    }
  }
  else {// if not the same letter rating, then the year doesn't matter:
    return strcmp(fencerA->rating, fencerB->rating);
  }
}

void make_pools(struct fencer * fclist, struct referee * rlist) {
  int n_fencers = count_fencers(fclist); //count number of fencers attending
  int n_refs = count_referees(rlist); //count number of refs available
  int n_pools = n_fencers / 5; //approx 5 fencers per pool
  if (n_pools > n_refs) { //if there aren't enough referees
    n_pools = n_refs; //cut it down until one ref, one pool
  }
  qsort(fclist, n_fencers, sizeof(*fclist), comp_fc); //qsort fxn, named after quicker sort
  print_fens(fclist);


}


struct referee * referee_list(char * filename) {
  int fd, nbytes, counter; //file descriptor and number of bytes read
  char * line; //stores each line of csv
  char * info; //stores ref info of each line of csv
  char bufferd[BUFFER_SIZE];

  fd = open(filename, O_RDONLY); //open ref list
  nbytes = read(fd, bufferd, sizeof(bufferd)); //read ref list into buffer
  if (nbytes < 0) { //if nbytes read returned -1
    printf("couldn't read referee file\n");
    printf("%s\n", strerror(errno));
  }
  struct referee * current = malloc(1000);//array of referee structs?
  char * buffer = strdup(bufferd); //it wasnt working with the char []
  line = strsep(&buffer, "\n");
  printf("line: %s\n", line);
  counter = 0;
  while (strlen(line) != 0) { //go through each referee in csv
    current[counter].last_name = strsep(&line, ",");
    // printf("%s\n", line);
    current[counter].first_name = strsep(&line, ",");
    // printf("%s\n", line);
    current[counter].country = strsep(&line, ",");
    // printf("%s\n", line);
    current[counter].club = strsep(&line, ",");
    // printf("%s\n", line);
    counter++;
    line = strsep(&buffer, "\n");
    //printf("%lu\n", strlen(line));
    // printf("line end: %s\n", line);
  }

  return current;
}

struct fencer * fencer_list(char * filename) {
  int fd, nbytes, counter;
  char * line, info;
  char bufferd[BUFFER_SIZE];
  fd = open(filename, O_RDONLY); //open fencer list
  nbytes = read(fd, bufferd, sizeof(bufferd)); //read ref list into buffer
  if (nbytes < 0) { //if nbytes read returned -1
    printf("couldn't read referee file\n");
    printf("%s\n", strerror(errno));
  }
  struct fencer * current = malloc(1000);//array of fencer structs
  char * buffer = strdup(bufferd);
  line = strsep(&buffer, "\n");
  counter = 0;
  while (strlen(line) != 0) {
    current[counter].last_name = strsep(&line, ",");
    current[counter].first_name = strsep(&line, ",");
    current[counter].country = strsep(&line, ",");
    current[counter].club = strsep(&line, ",");
    current[counter].rating = strsep(&line, ",");
    counter++;
    line = strsep(&buffer, "\n");
  }

  return current;
}

int main() {

  printf("referees: \n");
  struct referee * refs = malloc(1000);
  refs = referee_list("ref_list.csv");
  print_refs(refs);
  printf("fencers: \n");
  struct fencer * fens = malloc(1000);
  fens = fencer_list("fencer_list.csv");
  print_fens(fens);
  make_pools(fens, refs);
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
  free(refs);
  exit(1);
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
