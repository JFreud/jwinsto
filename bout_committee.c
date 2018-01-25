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


int debug = 0; //if on uses hardcoded pool, if off will run program normally
int n_pools; //num of pools created

static void sighandler(int signo) {
  if (signo == SIGINT) {
    // close(client_socket);
    // shutdown(listen_socket, SHUT_RDWR);
    exit(0);
  }
}





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

void print_pool(struct pool_fencer * fclist) {
  while (fclist->last_name != NULL) { //iterates through fencers by checking if last name exists
    printf("\nfname: %s\n", fclist->first_name);
    printf("lname: %s\n", fclist->last_name);
    printf("victories: %d\n", fclist->victories);
    printf("ts: %d\n", fclist->ts);
    printf("tr: %d\n", fclist->tr);
    printf("ind: %d\n", fclist->ind);
    printf("place: %d\n", fclist->plc);
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

int count_pool(struct pool_fencer * fclist) {
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
  n_pools = n_fencers / 5; //approx 5 fencers per pool
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
  int n_fencers = count_pool(pf);
  for (; i < n_fencers; i++) {
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

struct pool_fencer * test_pool(struct pool_fencer * pool) { //so we don't have to keep adding info
  pool[0].first_name = "thompson";
  pool[0].last_name = "hui";
  pool[0].victories = 4;
  pool[0].ts = 20;
  pool[0].tr = 0;
  pool[0].ind = 20;
  pool[0].plc = 1;

  pool[1].first_name = "yehia";
  pool[1].last_name = "ellis";
  pool[1].victories = 0;
  pool[1].ts = 0;
  pool[1].tr = 20;
  pool[1].ind = -20;
  pool[1].plc = 5;

  pool[2].first_name = "alice";
  pool[2].last_name = "liu";
  pool[2].victories = 3;
  pool[2].ts = 15;
  pool[2].tr = 5;
  pool[2].ind = 10;
  pool[2].plc = 2;

  pool[3].first_name = "benny";
  pool[3].last_name = "he";
  pool[3].victories = 2;
  pool[3].ts = 10;
  pool[3].tr = 10;
  pool[3].ind = 0;
  pool[3].plc = 3;

  pool[4].first_name = "eric";
  pool[4].last_name = "zhang";
  pool[4].victories = 1;
  pool[4].ts = 5;
  pool[4].tr = 15;
  pool[4].ind = -10;
  pool[4].plc = 4;

  return pool;
}


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

int comp_seed(const void * a, const void * b) { //compare two pool_fencers for qsort
  struct pool_fencer * fencerA = (struct pool_fencer *)a;
  struct pool_fencer * fencerB = (struct pool_fencer *)b;
  if (fencerA->victories > fencerB->victories) {
    return -1;
  }
  else if (fencerA->victories < fencerB->victories) {
    return 1;
  }
  else {//same number/percentage of victories
    if (fencerA->ind >= fencerB->ind) {
      return -1;
    }
    else {
      return 1;
    }
  }
}

struct fencer * convert_fcinfo(struct pool_fencer * seeded_fencers, struct fencer * fclist) { //convert pool_fencer back to regular struct
  int n_fencers = count_pool(seeded_fencers);
  struct fencer * seeded_info = malloc(1000);
  int seed_index = 0;
  while (seeded_fencers->last_name != NULL) {
    int i = 0;
    while(strcmp((*seeded_fencers).last_name, fclist[i].last_name) != 0) {//we really gotta hope fencers dont have the same last name
      i++;
    }
    // printf("%s\n", (*seeded_fencers).last_name);
    // printf("%s\n", fclist[i].last_name);
    // if (i >= n_fencers) {
    //   printf("ya dun goof\n");
    //   exit(0);
    // }
    seeded_info[seed_index] = fclist[i];
    seed_index++;
    seeded_fencers++;
  }
  return seeded_info;
}

struct pool_fencer * seed(struct pool_fencer ** pools) { //return post-pool seeding of fencers
  struct pool_fencer * seeded_fencers = malloc(1000); //one big thing
  char * lname = malloc(50);
  int n_fencers = 0;
  int sfen_index = 0; //seeded fencer index
  while (*pools != NULL) { //look at this O(n^2) magic
    printf("\n\n=====SEEDING=====\n\n"); //go through every pool
    n_fencers = n_fencers + count_pool(*pools);
    while ((lname = (*pools)->last_name) != NULL) { //every fencer in the pool
      seeded_fencers[sfen_index] = **pools;
      sfen_index++;
      (*pools)++;
    }
    pools++;
  }
  printf("nfencers: %d\n", n_fencers);
  qsort(seeded_fencers, n_fencers, sizeof(*seeded_fencers), comp_seed);
  free(lname);
  return seeded_fencers;
}

void print_seeding(struct fencer * seed_list) {
  printf("Name\t\tClub\tRating\tCountry\n");
  int count = 1;
  while (seed_list->last_name != NULL) {
    printf("%d. Name: %s %s\tClub: %s\tRating:%s\tCountry: %s\n", count, seed_list->first_name, seed_list->last_name, seed_list->club, seed_list->rating, seed_list->country);
    seed_list++;
    count++;
  }
}

int main() {

  signal(SIGINT, sighandler);
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
  // int listen_socket = committee_setup(); //creates listening socket
  printf("listening socket\n");
  int listen_socket = committee_setup(); //creates listening socket
  struct pool_fencer ** all_pools = malloc(1000);
  int pool_num = 0;
  while (pool_num <= n_pools) { //change to while not all pools filled out

    int client_socket = committee_connect(listen_socket); //runs accept call to connect committee with client
    if (fork()) { //forking server!
      printf("forked!\n");
      close(client_socket); //end connection
    } //parent
    else {//child
      struct pool_fencer * single_pool = malloc(500);
      single_pool = subcommittee(client_socket, pools); //the forked child will deal with the client
      print_pool(single_pool);
      all_pools[pool_num] = single_pool;
      // printf("last uh name: %s\n", all_pools[pool_num]->last_name);
      pool_num++;
      printf("line 392?\n");
      // print_pool(all_pools[pool_num]); <----- WHY DOESN'T THIS LINE WORK??
      printf("post-print\n");
      struct pool_fencer * seeded = seed(all_pools);
      printf("\n\n=======POST-POOL SEEDING ORDER======\n\n");
      print_pool(seeded);
      struct fencer * seeded_info = convert_fcinfo(seeded, fens);
      printf("\n\n=======REAL SEEDING LIST=======\n\n");
      // print_fens(seeded_info);
      print_seeding(seeded_info);
      //now convert back to struct fencer ig
    }
  }
  free(refs);
  free(fens);
  free(pools);
  free(all_pools);
  exit(1);
}

void print_bout(struct bout tada) { //prints the bout struct
  printf("Ref: %s\n", tada.referee);
  printf("Winner: %s\n", tada.winner);
  printf("Loser: %s\n", tada.loser);
  printf("win_score: %d\n", tada.win_score);
  printf("lose_score: %d\n", tada.lose_score);
}

struct pool_fencer * create(struct fencer * assigned) { //create pool with pool info instead of fencer info

  // printf("plsA !\n");
  int num_fencers = count_fencers(assigned);
  struct pool_fencer * pool_fencerz = malloc(1000);
  while (assigned -> last_name != NULL) {
    // printf("pls!\n");
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
  // printf("pls ;(\n");
  return pool_fencerz - num_fencers; //return pointer to beginning of pool
}

int compute_n_bouts(struct pool_fencer * pool) { //compute how many bouts in a pool
  int n_fencers = count_pool(pool); //count fencers in pool
  printf("NFENCERS: %d\n", n_fencers);
  int n_bouts = 0;
  for (;n_fencers > 0; n_fencers--) {
    printf("nf var: %d\n", n_fencers-1);
    n_bouts = n_bouts + (n_fencers - 1); //sum of unique individuals each fencer fences
  }
  return n_bouts;
}

struct pool_fencer * subcommittee(int client_socket, struct fencer ** assigned_pool) {
  char buffer[BUFFER_SIZE];
  int j, n_bouts;
  struct bout received_bout;
  struct bout bout_array[1000];
  struct referee * ref_list = referee_list("ref_list.csv");
  struct fencer * real_assigned_pool = malloc(1000);
  int bout_count = 0;
  char * input, * type, * referee;

  print_refs(ref_list);
  printf("ho\n");

  read(client_socket, buffer, sizeof(buffer));
  input = strdup(buffer);
  type = strsep(&input, ":");
  printf("%s\n", type);
  if (strcmp(type, "ref") == 0) { //if input is ref name fill out that part of bout info
      int i = 0;
      while(strcmp(ref_list[i].last_name, input) != 0){ //ALERT: WILL RUN FOREVER IF TYPO IN REF NAME
          i++;
      }
      real_assigned_pool = assigned_pool[i]; //set buffer to the pool the ref is assigned to
      referee = input;
  }
  else { //type != ref for some reason
    printf("something went wrong\n");
    printf("%s\n", strerror(errno));
    exit(1);
  }
  int num_fencers = count_fencers(real_assigned_pool);
  printf("%s\n", referee);

  struct pool_fencer * pool = malloc(1000);
  pool = create(real_assigned_pool);

  if (debug == 1) {
    pool = test_pool(pool);
    close(client_socket);
    free(real_assigned_pool);
    display_pools(pool);
    printf("n_bouts: %d\n", compute_n_bouts(pool));
    return pool;
  }
  printf("Not debugging\n");

  write(client_socket, buffer, sizeof(buffer)); //tell client what was received so it can print and user can verify

  n_bouts = compute_n_bouts(pool);

  printf("Opa\n");
  while(read(client_socket, buffer, sizeof(buffer)) && bout_count < n_bouts) { //read from client stream
    received_bout.referee = referee;
    input = strdup(buffer);
    type = strsep(&input, ":");
    printf("%s\n", type);
    if (strcmp(type, "win") == 0) { //if input is winner name fill out that part of bout info
      received_bout.winner = input;
    }
    else if (strcmp(type, "wsc") == 0) { //if input is winner score fill out that part of bout info
      received_bout.win_score = atoi(input);
    }
    else if (strcmp(type, "los") == 0) { //if input is loser name fill out that part of bout info
      received_bout.loser = input;
    }
    else if (strcmp(type, "lsc") == 0) { //if input is loser score fill out that part of bout info (last piece of information)
      received_bout.lose_score = atoi(input);
      print_bout(received_bout);
      bout_array[bout_count] = received_bout;
      bout_count++;
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
      printf("bout count: %d\n", bout_count);
      printf("nbouts: %d\n", n_bouts);
    }
    else { //incorrect input
      printf("Something isn't right\n");
      exit(0);
    }
    write(client_socket, buffer, sizeof(buffer)); //tell client what was received so it can print and user can verify
  }
  close(client_socket);
  printf("done with bouts\n");
  return pool;
}
