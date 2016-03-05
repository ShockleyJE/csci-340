#include <stdio.h>         // for printf()
#include <pthread.h>       // for pthread_xxx() routines
#include <time.h>
#include "mcsim.h"

// #include <limits.h>        //for modifying memory limits


double gcount;    // global counter -- a double to handle large sums (billions+)
long numits;      // global variable for number of iterations (see step 3 below)

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main( int argc, char** argv ) {

  gcount = 0;

  /*

  ---------------------------------------------------------------------
  Step-by-Step procedure for Monte carlo simulator to estimate PI
  ---------------------------------------------------------------------
  1. Create the following variables:
    - pointer to an array of pthread structures
    - counting variables for loops
    - thread id
    - exit status of joined thread
    - number of threads to run
    - maximum number of threads (hint: rlimit)
    - nanosecond start and stop time structs (hint: timespec)
  2. Get number of threads input by user from argv[1]
  3. Get number of iterations input by user from argv[2]
  4. Get the maximum number of threads the OS can create (hint: getrlimit function)
  5. If are number of threads > maximum OS threads then goto 14, else goto step 6
  6. Allocate an array of pthread structures using number of threads input by user (see step 2)
  7. Get start time of simulation and store in time struct
  8. Use a loop to create a pthread for each index position in pthread array (created in step 5), if an error occurs during thread creation - exit simulation program immediately (hint: pthread_create function)
  9. Use a loop to join each pthread in created in the pthread array, if an error occurs during thread join operation - exit simulation program immediately (hint: pthread_join function)
  10. Get stop time of simulation and store in time struct
  11. Print out the:
    - value of PI using 6 decimal places
    - value stored in global variable gcount using 0 decimal places
    - time in seconds required to run the simulation using 4 decimal places (hint: mydifftime function)
  12. Free memory allocated for pthread array
  13. Call pthread terminate function (hint: pthread_exit function)
  14. exit simulation program

  ---------------------------------------------------------------------
  Example input and output of Monte carlo simulator to estimate PI
  ---------------------------------------------------------------------

    # ./hw5 300 100   // i.e. run simulatior using 300 threads and 100 iterations

      monte carlo value of PI = 3.141200
      value of count = 23559
      time in seconds = 0.0761

  */

    //  1. Create the following variables:
   // - pointer to an array of pthread structures
  pthread_t *threads;

  int i,            //    - counting variables for loops
  thread_id = 0,   //    - thread id
  exit_status,    //    - exit status of joined thread
  num_threads,   //    - number of threads to run
  error;          //    - error code for exit
  double time_elapsed;

  struct rlimit max_threads;
  struct timespec start, end;

  //  2. Get number of threads input by user from argv[1]
  num_threads = atoi(argv[1]);

  //  3. Get number of iterations input by user from argv[2]
  numits = atoi(argv[2]);

  //  4. Get the maximum number of threads the OS can create (hint: getrlimit function)
  getrlimit(RLIMIT_NPROC, &max_threads);

  //  5. If are number of threads > maximum OS threads then goto 14, else goto step 6
  if (num_threads > max_threads.rlim_cur)
  {
    exit(1);
  }

  //  6. Allocate an array of pthread structures using number of threads input by user (see step 2)
  threads = (pthread_t*) malloc(sizeof(pthread_t) * num_threads);

  //  7. Get start time of simulation and store in time struct
  clock_gettime(CLOCK_REALTIME, &start);

  //  8. Use a loop to create a pthread for each index position in pthread array (created in step 5)
  for (i = 0; i < num_threads; i++)
  {

    if ((error = pthread_create(&(threads[i]), NULL, (void*) th_routine, (void*) thread_id++)) != 0)
    {
      //if an error occurs during thread creation - exit simulation program immediately
      exit(error);
    }

  }

  //  9. Use a loop to join each pthread in created in the pthread array
  for (i = 0; i < num_threads; i++)
  {
    if ((error = pthread_join(threads[i], NULL)) != 0)
    {
      //if an error occurs during thread join operation - exit simulation program immediately
      exit(error);
    }
  }

  //  10. Get stop time of simulation
  clock_gettime(CLOCK_REALTIME, &end);
  //  store in time struct
  time_elapsed = mydifftime(&start, &end);

  //print data
  printf("monte carlo value of PI: %.6f\nvalue of count: %.0f\ntime in seconds: %.4f\n\n", (4 * gcount/(numits * num_threads)), gcount, time_elapsed / 1000000000);

  //  12. Free memory allovalue of countcated for pthread array
  free(threads);

  //  13. Call pthread terminate function (hint: pthread_exit function) 
  //  NOTE: is it considered best practice to terminate a thread from main?

  //  14. exit simulation program
  return 0;

} // end main function
