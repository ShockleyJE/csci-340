#include <stdio.h>    /* for printf */
#include <stdlib.h>   /* for string to integer conversion, random numbers */
#include "mem.h"

/*
  The main program will accept four paramemters on the command line.
  The first parameter is the memory size.  The second parameter is the
  duration of the each simulation run.  The third parameter is the
  number of times to repeat each experiment (ie. number of duration). The
  fourth parameter is a random number seed. Here is an example command
  line:

  ./hw7 1000 3000 100 1235

  This means that your program should initialize physical memory to
  1,000 units, perform 100 duration with each run taking 3000 units of
  time, and the random number generator should be seeded (one time)
  with the value 1235.
*/

/* min & max duration of use for a block of memory */
#define MIN_DURATION      3
#define MAX_DURATION     25

/* min & max memory request sizes */
#define MIN_REQUEST_SIZE    3
#define MAX_REQUEST_SIZE  100

int main(int argc, char** argv)
{

  //check for 4 parameters at command line
  if( argc != 5)
  {
    fprintf(stderr, "%s arguments, expected 4. Terminating\n", );
    exit(1);
  }
  //bring in arguments from command line
  int memorysize= atoi(argv[1]);
  int duration= atoi(argv[2]);
  int iterations= atoi(argv[3]);
  int seedr= atoi(argv[4]);
  //initialize memory bounds and free memory limit
  meminit(memorysize);
  //seed the random
  srand(seedr);
  //set ranges from defined constants
  int duration_range = MAX_DURATION - MIN_DURATION;
	int size_range = MAX_REQUEST_SIZE - MIN_REQUEST_SIZE;

  //keep track of which which method we are using
  // 0= First fit, 1= Next fit, 2= best fit
  int whichFit;
  char *fit_string;
  enum mem_strategies strategy;

  for (whichFit = 0; whichFit <= 2; whichFit++){
		fragmentations = 0;
		misses = 0;
		probes = 0;
		mem_clear();
		switch (whichFit){
			case 0:
				strategy = FIRST;
				fit_string = "FIRST\0";
				break;
			case 1:
				strategy = NEXT;
				fit_string = "NEXT\0";
				break;
			case 2:
				strategy = BEST;
				fit_string = "BEST\0";
				break;
			default:
				fprintf(stderr, "enumerator error, shouldn't happen\n");
				exit(1);
		}
		for (i = 0; i < duration; i++){
			// each iteration in this loop is a full test with a single
			// placement strategy

			for (j = 0; j < iterations; j++){
				// each iteration in this loop represents one time unit

				// use one set of memory for all of *iterations*
				// then clear it out and do it again (outer loop)

				// get random number in the right range
				// by getting a num between 0-range, then adding min
				int duration_temp = (rand() % (duration_range + 1)) + MIN_DURATION;
   			int size_temp = (rand() % (size_range + 1)) + MIN_REQUEST_SIZE;
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
	printf("%s:\tavg fragmentation: %.3f\tavg failures: %.3f\tavg probes: %.5f\n", fit_string, ((double) fragmentations) / ((double) duration), ((double) misses)/((double) duration), ((double) probes)/((double) duration));

	}
	mem_free();

  return 0;
}
