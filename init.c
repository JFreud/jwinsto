#include "network.h"

int main(){

  char buffer[BUFFER_SIZE];
  int type, status;

  printf("Are you a fencer or referee? Enter appropriate number\n\t1) Fencer\n\t2) Referee\n"); //determines ref or fencer
  fgets(buffer, sizeof(buffer), stdin); //takes input as number

  type = atoi(buffer); //converts input to integer
  type--; //subtracts to make the input 0 or 1 for ease of use

  if (type){ //referee
    printf("You have selected referee\n");
    status = system("./referee"); //launch referee client
  }

  else{ //fencer
    printf("You have selected fencer\n");
    status = system("./fencer"); //launch fencer client
    }

  return 0;
}
