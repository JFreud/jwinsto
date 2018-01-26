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
struct bout * subDE(int client_socket, struct bout * curDEs);
struct bout * global_DE_list;
void display_DEs(int num_fencers, int tab);
char * send_pool(struct pool_fencer * pool);

int easy_pool = 1; //if on uses hardcoded pool, if off will run program normally
int easy_DE = 1;
int n_pools; //num of pools created

static void sighandler(int signo) {
  if (signo == SIGINT) {
    // close(client_socket);
    // shutdown(listen_socket, SHUT_RDWR);
    exit(0);
  }
}


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
        int count = 0;
        while (count < n_pools) {
          printf("\nSPRINTING POOL: %d\n", count);
          print_fens(pools[count]);
          count++;
        }
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
  printf("=====================================================================================\n");
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
    printf("V: %d %%   TS: %d   TR: %d   Ind: %d\tPl: %d \n", pf[j].victories, pf[j].ts, pf[j].tr, pf[j].ind, pf[j].plc);

    printf("------------------------------------------------------------------------------------\n");
  }
  printf("\n");
  free(name);
}


int questionmark(int n){ //this will not work for greater than 2^12 fencers

    return ceil(log2(n) - floor(log2(n)));
}

struct bout * first_DE(struct fencer * seeded_fencers) {
  struct bout * DE_list = malloc(1000); //list of bouts for this round of DEs
  int n_fencers = count_fencers(seeded_fencers); //number of fencers
  int i = 0;
  while (pow(2, i) <= n_fencers) {
    i++;
  }
  int cutoff = pow(2, i-1); //factor of two, less than n_fencers
  int not_bye = (n_fencers - cutoff) * 2;
  int bye = n_fencers - not_bye;
  printf("tableau: T%d, #fencers: %d, cutoff: %d, not bye: %d, n_BYEs: %d\n", cutoff, n_fencers, cutoff, not_bye, bye);

    struct referee * ref_list = referee_list("ref_list.csv");
    int n_referees = count_referees(ref_list);

    int referee_index = 0;

    int z = 0;
    for(; z < (n_fencers - cutoff); z++){

        printf("Can we get here?\n");

        struct bout * next = malloc(200);
        next->winner = seeded_fencers[n_fencers - (not_bye) + z ].last_name;
        next->loser = seeded_fencers[n_fencers - z - 1].last_name;
        next->referee = ref_list[referee_index % n_referees].last_name;
        referee_index++;
            printf("\n\nDE bout %d\n", z);

        print_bout(*next);

        DE_list[z] = *next;

    }

  return DE_list;
}

struct bout * later_DEs(struct fencer * seeded_fencers){

    struct bout * DE_list = malloc(1000);
    struct bout * post_bye = malloc(1000);
    struct bout * pre_bye = malloc(1000);
    int n_fencers = count_fencers(seeded_fencers);
    printf("mantis\n");
    struct referee * ref_list = referee_list("ref_list.csv");
    printf("Po\n");
    int n_referees = count_referees(ref_list);
    printf("Tigress?\n");

    int referee_index = 0;

    printf("testing questionmark: %d\n", questionmark(16));

    if (questionmark(n_fencers)){
         return first_DE(seeded_fencers);
    }

    else{

        int z = 0;
        for(; z < n_fencers/2; z++){

        struct bout * next = malloc(200);
        next->winner = seeded_fencers[z].last_name;
        next->loser = seeded_fencers[n_fencers - z - 1].last_name;
        next->referee = ref_list[referee_index % n_referees].last_name;
        next->win_score = 0;
        next->lose_score = 0;
        referee_index++;
            //printf("\n\nDE bout %d\n", z);
        //print_bout(*next)

        DE_list[z] = *next;
            global_DE_list[z] = *next;
        }
        printf("ho\n");
        printf("n fencers: %d", n_fencers);
        display_DEs(n_fencers, n_fencers/2);
        printf("hi\n");
        return DE_list;
    }

}



struct pool_fencer * test_pool0() { //so we don't have to keep adding info
  struct pool_fencer * pool = malloc(1000);
  pool[0].first_name = "thompson";
  pool[0].last_name = "hui";
  pool[0].victories = 5;
  pool[0].ts = 20;
  pool[0].tr = 0;
  pool[0].ind = 20;
  pool[0].plc = 1;

    pool[2].first_name = "alice";
  pool[2].last_name = "liu";
  pool[2].victories = 4;
  pool[2].ts = 15;
  pool[2].tr = 5;
  pool[2].ind = 10;
  pool[2].plc = 2;

    pool[3].first_name = "jerome";
  pool[3].last_name = "freudenberg";
  pool[3].victories = 3;
  pool[3].ts = 10;
  pool[3].tr = 10;
  pool[3].ind = 0;
  pool[3].plc = 3;

    pool[4].first_name = "max";
  pool[4].last_name = "chan";
  pool[4].victories = 2;
  pool[4].ts = 5;
  pool[4].tr = 15;
  pool[4].ind = -10;
  pool[4].plc = 4;

  pool[1].first_name = "yehia";
  pool[1].last_name = "ellis";
  pool[1].victories = 1;
  pool[1].ts = 0;
  pool[1].tr = 20;
  pool[1].ind = -20;
  pool[1].plc = 5;

  pool[5].first_name = "han";
  pool[5].last_name = "nam";
  pool[5].victories = 0;
  pool[5].ts = 0;
  pool[5].tr = 20;
  pool[5].ind = -20;
  pool[5].plc = 6;

  return pool;
}

struct pool_fencer * test_pool1() { //so we don't have to keep adding info
    struct pool_fencer * pool = malloc(1000);

  pool[0].first_name = "phillip";
  pool[0].last_name = "shin";
  pool[0].victories = 5;
  pool[0].ts = 20;
  pool[0].tr = 0;
  pool[0].ind = 20;
  pool[0].plc = 1;

    pool[2].first_name = "lauren";
  pool[2].last_name = "kim";
  pool[2].victories = 4;
  pool[2].ts = 15;
  pool[2].tr = 5;
  pool[2].ind = 10;
  pool[2].plc = 2;

    pool[3].first_name = "sarantos";
  pool[3].last_name = "soumakis";
  pool[3].victories = 3;
  pool[3].ts = 10;
  pool[3].tr = 10;
  pool[3].ind = 0;
  pool[3].plc = 3;

    pool[4].first_name = "katie";
  pool[4].last_name = "hwang";
  pool[4].victories = 2;
  pool[4].ts = 5;
  pool[4].tr = 15;
  pool[4].ind = -10;
  pool[4].plc = 4;

  pool[1].first_name = "eric";
  pool[1].last_name = "zhang";
  pool[1].victories = 1;
  pool[1].ts = 0;
  pool[1].tr = 20;
  pool[1].ind = -20;
  pool[1].plc = 5;

  return pool;
}

struct pool_fencer * test_pool2() { //so we don't have to keep adding info
    struct pool_fencer * pool = malloc(1000);
  pool[0].first_name = "rachel";
  pool[0].last_name = "koo";
  pool[0].victories = 5;
  pool[0].ts = 20;
  pool[0].tr = 0;
  pool[0].ind = 20;
  pool[0].plc = 1;

    pool[2].first_name = "taaseen";
  pool[2].last_name = "ali";
  pool[2].victories = 4;
  pool[2].ts = 15;
  pool[2].tr = 5;
  pool[2].ind = 10;
  pool[2].plc = 2;

    pool[3].first_name = "lea";
  pool[3].last_name = "levi";
  pool[3].victories = 3;
  pool[3].ts = 10;
  pool[3].tr = 10;
  pool[3].ind = 0;
  pool[3].plc = 3;

    pool[4].first_name = "benny";
  pool[4].last_name = "he";
  pool[4].victories = 2;
  pool[4].ts = 5;
  pool[4].tr = 15;
  pool[4].ind = -10;
  pool[4].plc = 4;

  pool[1].first_name = "cedric";
  pool[1].last_name = "mecke";
  pool[1].victories = 1;
  pool[1].ts = 0;
  pool[1].tr = 20;
  pool[1].ind = -20;
  pool[1].plc = 5;

  return pool;
}

struct bout * test_DE0() {
  struct bout * DE = malloc(500);

  DE[0].referee = "aziz";
  DE[0].winner = "shin";
  DE[0].loser = "nam";
  DE[0].win_score = 15;
  DE[0].lose_score = 3;

  DE[1].referee = "aziz";
  DE[1].winner = "ali";
  DE[1].loser = "mecke";
  DE[1].win_score = 15;
  DE[1].lose_score = 4;

  DE[2].referee = "aziz";
  DE[2].winner = "freudenberg";
  DE[2].loser = "chan";
  DE[2].win_score = 15;
  DE[2].lose_score = 14;

  return DE;
}

struct bout * test_DE1() {
  struct bout * DE = malloc(500);

  DE[0].referee = "nazarova";
  DE[0].winner = "koo";
  DE[0].loser = "zhang";
  DE[0].win_score = 15;
  DE[0].lose_score = 3;

  DE[1].referee = "nazarova";
  DE[1].winner = "liu";
  DE[1].loser = "he";
  DE[1].win_score = 15;
  DE[1].lose_score = 4;

  DE[2].referee = "nazarova";
  DE[2].winner = "levi";
  DE[2].loser = "soumakis";
  DE[2].win_score = 15;
  DE[2].lose_score = 14;

  return DE;
}

struct bout * test_DE2() {
  struct bout * DE = malloc(500);

  DE[0].referee = "chan";
  DE[0].winner = "hui";
  DE[0].loser = "ellis";
  DE[0].win_score = 15;
  DE[0].lose_score = 3;

  DE[1].referee = "chan";
  DE[1].winner = "kim";
  DE[1].loser = "hwang";
  DE[1].win_score = 15;
  DE[1].lose_score = 4;

  return DE;
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

struct fencer find_fcinfo(char * winner, struct fencer * fclist) {
  while(fclist->last_name != NULL) {
    if(strcmp((*fclist).last_name, winner) == 0) {
      // printf("cand: %s\n", (*fclist).last_name);
      return *fclist;
    }
    fclist++;
  }
  printf("The winner is not actually in the tournament heh\n");
  return *(fclist - 1); //shouldn't get here
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
  printf("   Name\t\t\t\tClub\t\tRating\t\tCountry\n");
    printf("_______________________________________________________________________________\n");
  int count = 1;
  while (seed_list->last_name != NULL) {

      int name_length;
    name_length = strlen(seed_list->first_name) + strlen(seed_list->last_name);
      printf("%d.", count);
      if(count <= 9)
          printf(" ");
      printf(" Name: %s %s", seed_list->first_name, seed_list->last_name);
    //printf("%d", name_length);
    for (; name_length < 15; name_length++) {//spaces end at same point
      printf(" ");
    }
    printf("\t");


    printf("Club: %s\tRating:%s\tCountry: %s\n", seed_list->club, seed_list->rating, seed_list->country);
    seed_list++;
    count++;
  }
}

struct fencer * convert_winners (struct bout ** all_filled) {
  struct fencer * winners = malloc(1000);
  int bigdex = 0;
  int index = 0;
  int smalldex = 0;
  printf("%s\n",all_filled[bigdex]->referee);
  printf("hi\n");
  while (all_filled[bigdex]) {
    smalldex = 0;
    printf("doing loop\n");
    while(all_filled[bigdex][smalldex].referee != NULL) {
      printf("get here?\n");
      winners[index].last_name = all_filled[bigdex][smalldex].winner;
      printf("what here\n");
      printf("winner: %s\n", winners[index].last_name);
      index++;
      smalldex++;
      printf("%d\n",smalldex);
    }
    bigdex++;
  }
  print_fens(winners);
  return winners;
}

int count_bouts (struct bout * curDEs) {
  int size = 0;
  while(curDEs->referee != NULL) {
    size++;
    curDEs++;
  }
  return size;
}

int main() {
  global_DE_list = malloc(2000);
  // signal(SIGINT, sighandler);
  printf("referees: \n");
  struct referee * refs = malloc(1000);
  refs = referee_list("ref_list.csv");
  print_refs(refs);
  int n_refs = count_referees(refs);
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
  int client_socket;
  if (easy_pool == 1){

      printf("Making fake pools: \n\n");

      all_pools[0] = test_pool0();
      all_pools[1] = test_pool1();
      all_pools[2] = test_pool2();

      printf("Here?\n");

  }
  else {
  while (pool_num <= n_pools) { //change to while not all pools filled out

    client_socket = committee_connect(listen_socket); //runs accept call to connect committee with client
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
      }
      pool_num++;
    }
  }
  // display_DEs(16, 8);
  struct pool_fencer * seeded = seed(all_pools);
  printf("\n\n=======POST-POOL SEEDING ORDER======\n\n");
  print_pool(seeded);
  printf("POST PRINT\n");
  struct fencer * seeded_info = convert_fcinfo(seeded, fens);
  printf("\n\n=======REAL SEEDING LIST=======\n\n");
  // print_fens(seeded_info);
  print_seeding(seeded_info);
    //print_pool(seeded);
    // send_pool(seeded);
  // printf("hi\n");
  struct bout * curDEs = later_DEs(seeded_info);
  int num_DEs = count_bouts(curDEs);
  struct fencer * winners;
  printf("num DEs: %d\n", num_DEs);
  struct bout ** all_filled = malloc(1000);
  char * top = malloc(500);
  if (easy_DE) {
    all_filled[0] = test_DE0();
    all_filled[1] = test_DE1();
    all_filled[2] = test_DE2();
    printf("Received all data\n");
    struct fencer * winners = convert_winners(all_filled);
    printf("converted winners\n");
    curDEs = later_DEs(winners);
    num_DEs = count_bouts(curDEs);
    printf("num DEs: %d\n", num_DEs);

  }
  while (num_DEs > 0) {
        int refdex = 0;
        // printf("hi\n");
        while (refdex < n_refs && refdex < num_DEs) {
          client_socket = committee_connect(listen_socket); //runs accept call to connect committee with client
          // printf("HAWIHFW\n");
            if (fork()) {
              printf("forked DE!\n");
              close(client_socket); //end connection
              while(1){}
            }
            else { //child
              struct bout * single_filled = malloc(500);
              single_filled = subDE(client_socket, curDEs);
              all_filled[refdex] = single_filled;
            }
          printf("num DEs: %d\n", num_DEs);
          printf("finished ref\n");
          refdex++;
        }
        if(num_DEs < n_refs) {
          int leftovers = n_refs - num_DEs;
          for (; leftovers <= n_refs; leftovers++) {
            all_filled[refdex] = NULL;
            refdex++;
          }
        }
    printf("Received all data\n");
    winners = convert_winners(all_filled);
    printf("converted winners\n");
    curDEs = later_DEs(winners);
    num_DEs = count_bouts(curDEs);
    printf("num DEs: %d\n", num_DEs);
    top = winners[0].last_name;
  }
  printf("AND THE WINNER IS... \n");
  struct fencer won = find_fcinfo(top, fens);
  printf("%s %s from %s\n", won.first_name, won.last_name, won.club);
  //now convert back to struct fencer ig
  free(refs);
  free(fens);
  free(pools);
  free(all_pools);
  free(all_filled);
  close(client_socket);
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

struct bout * subDE(int client_socket, struct bout * curDEs) {
  printf("===========SUB DE-ING=========\n");
  char buffer[BUFFER_SIZE];
  char * input = malloc(100);
  char * type = malloc(100);
  printf("waiting for read\n");
  read(client_socket, buffer, sizeof(buffer));
  printf("read\n");
  struct bout received_bout;
  struct bout * next_tableau = malloc(1000);
  struct referee this_ref;
  input = strdup(buffer);
  type = strsep(&input, ":");
  printf("%s\n", type);
  int n_DEs = count_bouts(curDEs);
  int bouts_reffed = 0;
  int cur_reffed = 0;
  if (strcmp(type, "ref") == 0) { //if input is ref name fill out that part of bout info
      int i = 0;
      while(i < n_DEs){ //ALERT: WILL RUN FOREVER IF TYPO IN REF NAME
          if (strcmp(curDEs[i].referee, input) == 0) {
            bouts_reffed++;
            print_bout(curDEs[i]);
          }
          i++;
      }
      this_ref.last_name = input;
  }
  else { //type != ref for some reason
    printf("something went wrong\n");
    printf("%s\n", strerror(errno));
    exit(1);
  }
  printf("bouts assigned: %d\n", bouts_reffed);
  write(client_socket, buffer, sizeof(buffer)); //tell client what was received so it can print and user can verify
  while(read(client_socket, buffer, sizeof(buffer)) && cur_reffed < bouts_reffed) {
    received_bout.referee = this_ref.last_name;
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
      next_tableau[cur_reffed] = received_bout;
      printf("hak\n");
      cur_reffed++;
  }
  printf("cur reffed %d\n", cur_reffed);
  write(client_socket, buffer, sizeof(buffer)); //tell client what was received so it can print and user can verify
}
  return next_tableau;
}

char * send_pool(struct pool_fencer * pool) {
  char * buffer = malloc(1000);
    //printf("1\n");
  strcpy(buffer, "Fencers in your pool: \n");
    //printf("2\n");
    int z = 0;
  while (pool[z].last_name != NULL) {
    strcat(buffer, pool[z].last_name);
    strcat(buffer, "\n");
      z++;
  }

  printf("Sending: %s\n", buffer);
  return buffer;
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

  // strcpy(buffer, send_pool(pool));
  // write(client_socket, buffer, sizeof(buffer));

  if (easy_pool == 1) {
    pool = test_pool0();
    close(client_socket);
    free(real_assigned_pool);
    display_pools(pool);
    printf("n_bouts: %d\n", compute_n_bouts(pool));
    return pool;
  }
  printf("Not easy_poolging\n");

  write(client_socket, buffer, sizeof(buffer)); //tell client what was received so it can print and user can verify

  n_bouts = compute_n_bouts(pool);

  printf("Opa\n");
  while(read(client_socket, buffer, sizeof(buffer)) && bout_count < n_bouts ) { //read from client stream
    received_bout.referee = referee;
    input = strdup(buffer);
    type = strsep(&input, ":");
    printf("%s\n", type);
    display_pools(pool);
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
    if (bout_count == n_bouts) {
      strcpy(buffer, "finished");
    }
    write(client_socket, buffer, sizeof(buffer)); //tell client what was received so it can print and user can verify

  }
  close(client_socket);
  printf("done with bouts\n");
  return pool;
}

void display_DEs(int num_fencers, int tab){

    int z = num_fencers - tab*2;



    printf("\nTableau: %d           Tableau: %d (Prediction)\n\n", tab * 2, tab);

    for(; tab > 0; tab--){

        //printf("kann here?\n");

        for (;z < tab; z++){
            //printf("z: %d\n", z);
            //printf("wbu here 1?\n");

            int name_length1, name_length2;

            //printf("wbu here 2?\n");
            name_length1 = strlen(global_DE_list[z].winner);

            //printf("wbu here 3?\n");
            if (global_DE_list[z].win_score > 9)
                name_length1 ++;

            //printf("wbu here 4?\n");
            name_length2 = strlen(global_DE_list[z].loser);
            if (global_DE_list[z].lose_score > 9)
                name_length2 ++;

            printf("---%s", global_DE_list[z].winner);

            printf("(%d)", global_DE_list[z].win_score);
            for (; name_length1 < 13; name_length1++) {//spaces end at same point
                printf("-");
            }
            printf("\\");
            printf("\n   vs.\t\t   |---%s", global_DE_list[z].winner);

            name_length1 = strlen(global_DE_list[z].winner);
            for (; name_length1 < 13; name_length1++) {//spaces end at same point
                printf("-");
            }

            printf("\n---%s", global_DE_list[z].loser);

            printf("(%d)", global_DE_list[z].lose_score);

            for (; name_length2 < 13; name_length2++) {//spaces end at same point
            printf("-");
            }
            printf("/");
            printf("\n\n");

        }

    }


}
