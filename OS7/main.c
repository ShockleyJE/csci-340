#include <stdio.h>
#include <stdlib.h>

#include "mem.h"


/* minimum and maximum duration of use for an allocated block of memory */
#define MIN_DURATION      3
#define MAX_DURATION     25

/* minimum and maximum allocation request size */
#define MIN_REQUEST_SIZE    3
#define MAX_REQUEST_SIZE  100

/*
  The main program will accept four paramemters on the command line.
  The first parameter is the memory size.  The second parameter is the
  duration of the each simulation run.  The third parameter is the
  number of times to repeat each experiment (ie. number of runs). The
  fourth parameter is a random number seed. Here is an example command
  line:

  ./hw7 1000 3000 100 1235

  This means that your program should initialize physical memory to
  1,000 units, perform 100 runs with each run taking 3000 units of
  time, and the random number generator should be seeded (one time)
  with the value 1235.
*/

int main(int argc, char** argv){
	int memsize, runs, iterations, seed, method, i, j, sizerange, durationrange, dur, siz, total_frags = 0, total_misses = 0, total_probes = 0, result;
	enum mem_strategies strategy;
	char *strat_string;
	if (argc != 5){
    printf("expected 4 args, not %d\n", argc );
	  exit(1);
	}
	memsize = atoi(argv[1]);
  iterations = atoi(argv[2]);
	runs = atoi(argv[3]);
	seed = atoi(argv[4]);
	mem_init(memsize);

	srand(seed);
	// then mult the percentages by the appropriate scale:
	durationrange = MAX_DURATION - MIN_DURATION;
	sizerange = MAX_REQUEST_SIZE - MIN_REQUEST_SIZE;
	for (method = 0; method <= 2; method++){
		total_frags = 0;
		total_misses = 0;
		total_probes = 0;
		mem_clear();
		switch (method){
			case 0:
				strategy = FIRST;
				strat_string = "FIRSTFIT\0";
				break;
			case 1:
				strategy = NEXT;
				strat_string = "NEXTFIT\0";
				break;
			case 2:
				strategy = BEST;
				strat_string = "BESTFIT\0";
				break;
			default:
				fprintf(stderr, "enum error, shouldn't happen\n");
				exit(1);
		}
		for (i = 0; i < runs; i++){

			for (j = 0; j < iterations; j++){

				dur = (rand() % (durationrange + 1)) + MIN_DURATION;
   			siz = (rand() % (sizerange + 1)) + MIN_REQUEST_SIZE;
				result = mem_allocate(strategy, siz, dur);

				if (result == -1){
					total_misses += 1;
				}
				else {
					total_probes += result;
				}
				mem_single_time_unit_transpired();
			}
			total_frags += mem_fragment_count(3);
		}
	printf("%s:\n\tmean fragmentation count = %.3f\n\tmean number of fails = %.3f\n\tmean number of probes = %.5f\n", strat_string, ((double) total_frags) / ((double) runs), ((double) total_misses)/((double) runs), ((double) total_probes)/((double) runs));

	}
	mem_free();
  return 0;
}
