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
  number of times to repeat each experiment (ie. number of duration). The
  fourth parameter is a random number seedr. Here is an example command
  line:

  ./hw7 1000 3000 100 1235

  This means that your program should initialize physical memory to
  1,000 units, perform 100 duration with each run taking 3000 units of
  time, and the random number generator should be seeded (one time)
  with the value 1235.
*/

int main(int argc, char** argv){

	if (argc != 5){
		fprintf(stderr, "expected 4 args in form: memory size,  duration, iterations, seed\n", argc, argv[0]);
		exit(1);
	}

	int memory_size, iterations, duration, seedr, i, j, result;

	memory_size = atoi(argv[1]);
	duration = atoi(argv[2]);
	iterations = atoi(argv[3]);
	seedr = atoi(argv[4]);
	mem_init(memory_size);

	srand(seedr);
	// then mult the percentages by the appropriate scale:
	duration_range = MAX_DURATION - MIN_DURATION;
	size_range = MAX_REQUEST_SIZE - MIN_REQUEST_SIZE;

	//keep track of which which method we are using
  // 0= First fit, 1= Next fit, 2= best fit
	int method_switch;
	char *strategy_string;
	enum mem_strategies strategy;
	int  fragmentations = 0, misses = 0, probes = 0;
	int size_range, duration_range;
	int  duration_temp, size_temp;

	for (method_switch = 0; method_switch <= 2; method_switch++){
		fragmentations = 0;
		misses = 0;
		probes = 0;
		mem_clear();
		switch (method_switch){
			case 0:
				strategy = FIRST;
				strategy_string = "FIRST\0";
				break;
			case 1:
				strategy = NEXT;
				strategy_string = "NEXT\0";
				break;
			case 2:
				strategy = BEST;
				strategy_string = "BEST\0";
				break;
			default:
				fprintf(stderr, "enumerator error, shouldn't happen\n");
				exit(1);
		}
		for (i = 0; i < iterations; i++){

			for (j = 0; j < duration; j++){
				duration_temp = (rand() % (duration_range + 1)) + MIN_DURATION;
   			size_temp = (rand() % (size_range + 1)) + MIN_REQUEST_SIZE;
				result = mem_allocate(strategy, size_temp, duration_temp);

				if (result == -1){
					misses += 1;
				}
				else {
					probes += result;
				}
				mem_single_time_unit_transpired();
			}
			fragmentations += mem_fragment_count(3);
		}
	printf("%s:\tavg fragmentation: %.3f\tavg failures: %.3f\tavg probes: %.5f\n", strategy_string, ((double) fragmentations) / ((double) iterations), ((double) misses)/((double) iterations), ((double) probes)/((double) iterations));

	}
	mem_free();
  return 0;
}
