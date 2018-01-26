all: referee committee

referee: referee.o network.o
	gcc -o referee referee.o network.o

committee: bout_committee.o network.o
	gcc -o committee bout_committee.o network.o -lm

referee.o: referee.c network.h
	gcc -c referee.c

bout_committee.o: bout_committee.c network.h
	gcc -c bout_committee.c

network.o: network.c network.h
	gcc -c network.c

clean:
	rm *.o
	rm *~
