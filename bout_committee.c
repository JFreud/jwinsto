#include "network.h"

struct pool_fencer * subcommittee(int client_socket, struct fencer ** assigned_pool);
struct referee * referee_list();
struct fencer * fencer_list();
struct pool_fencer * create(struct fencer * assigned);
void print_refs(struct referee * rlist);
void print_fens(struct fencer * fclist);
int count_fencers(struct fencer * fclist);
int count_referees(struct referee * rlist);
int comp_fc(const void * a, const void * b);
struct fencer ** make_pools(struct fencer * fclist, struct referee * rlist);
void display_pools(struct pool_fencer * pf);
void print_pools(struct fencer ** pools);
void print_bout(struct bout tada);








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

struct fencer ** make_pools(struct fencer * fclist, struct referee * rlist) {
  int n_fencers = count_fencers(fclist); //count number of fencers attending
  int n_refs = count_referees(rlist); //count number of refs available
  int n_pools = n_fencers / 5; //approx 5 fencers per pool
  if (n_pools > n_refs) { //if there aren't enough referees
    n_pools = n_refs; //cut it down until one ref, one pool
  }
  qsort(fclist, n_fencers, sizeof(*fclist), comp_fc); //qsort fxn, named after quicker sort
  print_fens(fclist);

  struct fencer ** pools = malloc(sizeof(struct fencer) * 10 * n_pools); //creates n_pools with a max of 10 fencers per pool
  int i = 0;
  for (; i < n_pools; i++) {
    pools[i] = malloc(sizeof(struct fencer) * 10);
  }

  printf("HAIWD\n");

  int fendex = 0; //keeps track of fencer position in pool
  int fendex_reservoir = 0; //keeps track fo what fencer we're allocating from fclist
  printf("HAIWD\n");
  for (; fendex < n_fencers; fendex++) { //goes through each fencer for each pool. e.g. adds a fencer 1 to every pool, then a fencer 2 to every pool
    printf("HAIWD\n");
    int pool_index = 0; //keeps track of what pool is being changed
    for (; pool_index < n_pools; pool_index++) {
      printf("HAIWD\n");
      if (fclist[fendex_reservoir].last_name == NULL) { //all fencers have been assigned to a pool
	 printf("\nSPRINTING POOL 1: \n");
        print_fens(pools[0]);

        printf("\nSPRINTING POOL 2: \n");
        print_fens(pools[1]);
        return pools;
      }
      printf("fencer: %s\n", fclist[fendex_reservoir].first_name);
      //NEXT LINE PRONE TO SEG FAULTS
      printf("pool byte: %lu\n", sizeof(pools[pool_index][fendex]));
      printf("fencer byte: %lu\n", sizeof(fclist[fendex_reservoir]));
      printf("HAI\n");

        //Trying stuff

        printf("\nTESTING HERE\n");
        printf("\npool_index: %d\nfendex_reservoir: %d\nfendex: %d\n", pool_index, fendex_reservoir, fendex);

        //pools[0][0] = &temp; //*pools = pools[0]
        print_fens(&pools[pool_index][fendex]);

        printf("test?\n");

        pools[pool_index][fendex] = fclist[fendex_reservoir]; //takes next fencer from fclist (... this will cause first pool to be better than last pool)

        print_fens(pools[pool_index]);
        printf("Can we get to here?\n");

      printf("HAIWD\n");
      fendex_reservoir++;
      printf("reservoir: %d\n", fendex_reservoir);
    }
      printf("fendex: %d\n", fendex);
  }

    printf("n_pools: %d\n", n_pools);

    printf("\nPRINTING POOL 1: \n");
    print_fens(pools[0]);

    printf("\nPRINTING POOL 2: \n");
    print_fens(pools[1]);
  return pools;
}

void display_pools(struct pool_fencer * pf) {
  int i = 0, name_length = 0;
  char * name = malloc(500);
  printf("======================================================================\n");
  struct pool_fencer * cur_pool = pf;
  for (; i < 5; i++) {
    //printf("first: %s size: %lu  ", cur_pool[i].first_name, strlen(cur_pool[i].first_name));
    //printf("last: %s size: %lu", cur_pool[i].last_name, strlen(cur_pool[i].last_name));
    name_length = strlen(cur_pool[i].first_name) + strlen(cur_pool[i].last_name);
    printf("|| ");
    printf("%s, ", cur_pool[i].last_name);
    printf("%s", cur_pool[i].first_name);
    //printf("%d", name_length);
    for (; name_length < 30; name_length++) {//spaces end at same point
      printf(" ");
    }
    printf("|");

    int j = 0;
    while(strcmp(pf[j].last_name, cur_pool[i].last_name) != 0){
      j++;
    }
    printf("V: %d %%   TS: %d   TR: %d   Ind: %d   Pl: %d \n", pf[j].victories, pf[j].ts, pf[j].tr, pf[j].ind, pf[j].plc);

    printf("---------------------------------------------------------------------\n");
  }
  printf("\n");
  free(name);
}

// void do_bout(bout chosen)
//
//
// prompt score
// receive score
// return score
//
// void do_pool():
// ordered bout array depending on people;
// connect referee
// chosen bout
// prompt bouts
// receive bout chosen


void print_pools(struct fencer ** pools) {
  int pool_num = 0;
  while (pools != NULL) {
    printf("\nPOOL NUMBER: %d\n", pool_num);
    print_fens(*pools);
    pools++;
  }
  free(pools);
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
  close(fd);
  return current;
}

struct fencer * fencer_list(char * filename) {
  int fd, nbytes, counter;
  char * line, info;
  char bufferd[BUFFER_SIZE];
  fd = open(filename, O_RDONLY); //open fencer list

  nbytes = read(fd, bufferd, sizeof(bufferd)); //read fc list into buffer
  if (nbytes < 0) { //if nbytes read returned -1
    printf("couldn't read fencer file\n");
    printf("%s\n", strerror(errno));
  }
  struct fencer * current = malloc(1000);//array of fencer structs

  char * buffer = strdup(bufferd);
  line = strsep(&buffer, "\n");
  counter = 0;
  while (buffer != NULL) {
    printf("%s\n", line);
    current[counter].last_name = strsep(&line, ",");
    current[counter].first_name = strsep(&line, ",");
    current[counter].country = strsep(&line, ",");
    current[counter].club = strsep(&line, ",");
    current[counter].rating = strsep(&line, ",");
    counter++;
    line = strsep(&buffer, "\n");
  }
  printf("h3\n");
  return current;
}

int main() {

  
  printf("referees: \n");
  struct referee * refs = malloc(1000);
  refs = referee_list("ref_list.csv");
  print_refs(refs);
  printf("\nfencers: \n");
  struct fencer * fens = malloc(1000);
  struct fencer ** pools = malloc(1000);
  printf("hi\n");
  fens = fencer_list("fencer_list.csv");
  printf("hi2\n");
  print_fens(fens);
  pools = make_pools(fens,refs);
  // print_pools(make_pools(fens, refs));
  // signal(SIGINT, sighandler);
  int listen_socket = committee_setup(); //creates listening socket

  while (1) {
    int client_socket = committee_connect(listen_socket); //runs accept call to connect committee with client
    if (fork()) { //forking server!
      printf("forked!\n");
      close(client_socket); //end connection
    } //parent
    else {//child
      subcommittee(client_socket, pools); //the forked child will deal with the client
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

struct pool_fencer * create(struct fencer * assigned) {

  printf("plsA !\n");
  int num_fencers = count_fencers(assigned);
  struct pool_fencer * pool_fencerz = malloc(1000);
  while (assigned -> last_name != NULL) {
    printf("pls!\n");
    pool_fencerz->last_name = assigned->last_name;
    pool_fencerz->first_name = assigned->first_name;
    pool_fencerz->victories = 0;
    pool_fencerz->ts = 0;
    pool_fencerz->tr = 0;
    pool_fencerz->ind = 0;
    pool_fencerz->plc = 0;
    pool_fencerz++;
    assigned++;
  }
  printf("pls ;(\n");
  return pool_fencerz - num_fencers;
}

struct pool_fencer * subcommittee(int client_socket, struct fencer ** assigned_pool) {
  char buffer[BUFFER_SIZE];

//  struct bout this_bout;
  struct bout received_bout;
  struct bout bout_array[1000];//is this the number of bouts?
  struct referee * ref_list = referee_list("ref_list.csv");
  struct fencer * real_assigned_pool = malloc(1000);
  int bout_count = 0;

  print_refs(ref_list);
  printf("ho\n");
  read(client_socket, buffer, sizeof(received_bout));
  memcpy(&received_bout, buffer, sizeof(received_bout));
  write(client_socket, buffer, sizeof(received_bout));
  printf("ho\n");
  printf("name: %s\n", received_bout.winner);
  print_bout(received_bout);
  char * cur_referee = received_bout.referee;
  printf("%s\n", received_bout.referee);
  int i = 0;
  while(strcmp(ref_list[i].last_name, cur_referee) != 0){
      printf("%s\n", ref_list[i].last_name);
      printf("%s\n", cur_referee);
      i++;
  }
  printf("ho\n");
  real_assigned_pool = assigned_pool[i];
  printf("ho\n");
  int num_fencers = count_fencers(real_assigned_pool);
  printf("%s\n", cur_referee);

  printf("OOps\n");
  struct pool_fencer * pool = malloc(1000);
  pool = create(real_assigned_pool);

  printf("Opa\n");

  int j = 0;
  while(strcmp(real_assigned_pool[j].last_name, received_bout.winner) != 0) {
    j++;
  }
  pool[j].victories++;
  pool[j].ts += received_bout.win_score;
  pool[j].tr += received_bout.lose_score;
  pool[j].ind = pool[j].ts - pool[j].tr;

  printf("Opa\n");

  j = 0;
  while(strcmp(real_assigned_pool[j].last_name, received_bout.loser) != 0) {
    j++;
  }
  pool[j].ts += received_bout.lose_score;
  pool[j].tr += received_bout.win_score;
  pool[j].ind = pool[j].ts - pool[j].tr;

  printf("Opa\n");

  display_pools(pool);

  printf("Opa\n");


  while(read(client_socket, &received_bout, sizeof(received_bout))) { //read from client stream

    j = 0;
    while(strcmp(real_assigned_pool[j].last_name, received_bout.winner) != 0) {
      j++;
    }
    pool[j].victories++;
    pool[j].ts += received_bout.win_score;
    pool[j].tr += received_bout.lose_score;
    pool[j].ind = pool[j].ts - pool[j].tr;

    j = 0;
    while(strcmp(real_assigned_pool[j].last_name, received_bout.loser) != 0) {
      j++;
    }
    pool[j].ts += received_bout.lose_score;
    pool[j].tr += received_bout.win_score;
    pool[j].ind = pool[j].ts - pool[j].tr;

    display_pools(pool);

      /***

      char * input = strdup(buffer);
    char * type = strsep(&input, ":");
    printf("%s\n", type);



    if (strcmp(type, "ref") == 0) { //if input is ref name fill out that part of bout info
        int i = 0;
        while(strcmp(referee_list[i], input) != 0){
            i++;
        }

        buffer = pools[i]; //set buffer to the pool the ref is assigned to

        write(client_socket, buffer, sizeof(buffer)); //send pool info to ref

        //this_bout.referee = input;
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
    } ***/
    write(client_socket, buffer, sizeof(buffer)); //tell client what was received so it can print and user can verify
  }
  close(client_socket);
  return pool;
  exit(0);
}
