#include "dpsim.h"

//class notes
//signal in pthread kill is the same as the signal looked at before (sigkill, #9)
//maybe, he's not sure. So check on this


static const unsigned int NUM_PHILOSPHERS = 5;
static const unsigned int NUM_FORKS = 5;

static int forks[5];
static pthread_mutex_t mutex[5];
static pthread_t philosphers[5];


void* th_main( void* th_main_args ) {

	// ---------------------------------------
	// TODO: you add your implementation here

	// 1. Initialize all element values in the chopsticks array to -1
	int i;
	for (i = 0; i < NUM_CHOPSTICKS; i++)
	{
		chopsticks[i] = -1;
	}
	// 2. Create a thread for each philosopher (hint: use philosophers[5] array)
	for (i = 0; i < NUM_PHILOSOPHERS; i++)
	{
		if (pthread_create(&(philosophers[i]), NULL, th_phil, (void*) i) != 0)
		{
			// - If a error condition occurs, then immediately exit this function with status value 1.
			exit(1);
		}
		// 3. Execute a infinite loop that does the following: ...
		while (1){
				delay(100000);
				// If a deadlock condition is found then display deadlock
				if (isdead())
				{
					printf("Deadlock condition, terminating\n");
					//terminate the loop and goto step 4
					break;
				}
				// display which philosophers are eating
				printf("Philosopher(s) ");
				for (i = 0; i < NUM_CHOPSTICKS; i++)
				{
					if (chopsticks[i] == chopsticks[(i + 1) % 5]) printf("%d, ", i);
				}
				printf("are eating\n");
				// if (DEBUG){
				// 	printf("chopsticks = {");
				// 	for (i = 0; i < NUM_CHOPSTICKS; i++)
				// 	{
				// 		printf("%d ", chopsticks[i]);
				// 	}
				// 	printf("}\n");
				// }
			}
			// 4. Kill each philosopher thread
			for (i = 0; i < NUM_PHILOSOPHERS; i++)
			{
				pthread_kill(philosophers[i], 9);
			}
			// 5. Exit the main thread with status value equal to 0.
			pthread_exit(0);
	}

} // end th_main function


void* th_phil( void* th_phil_args ) {

	// 1. Get the philosopher id (hint: use th_phil_args)
	int id = (int)th_phil_args;
  // 2. Execute an infinite loop that does the following:
	while(1)
	{
		// - call the delay function for thinking (you specify nanosec sleep value)
		delay(100000);
		// - call the eat function (argument is the philosopher id)
		eat(id);
	}
} // end th_phil function

// This function is provided to you (i.e. do not modify).
void delay( long nanosec ) {

	struct timespec t_spec;

	t_spec.tv_sec = 0;
	t_spec.tv_nsec = nanosec;

	nanosleep( &t_spec, NULL );

} // end think function


void eat( int phil_id ) {

	//defining left chopstick for transparency
	int left = (phil_id + 1) % 5;

	//return if right chopstick is already taken
	if (chopsticks[phil_id] != -1 && chopsticks[phil_id] != phil_id) { return; }

	pthread_mutex_lock(&(mutex[phil_id]));
	// pick up chopstick phil_id(i.e. the right chopstick)
	chopsticks[phil_id] = phil_id;
	pthread_mutex_unlock(&(mutex[phil_id]));

	// delays for no more than 20,000 nanoseconds
	delay(19999);

	//return if left chopstick is already taken
	if (chopsticks[left] != -1) { return; }

	pthread_mutex_lock(&(mutex[left]));
	// pickup left chopstick
	chopsticks[left] = phil_id;
	pthread_mutex_unlock(&(mutex[left]));


	// After having picked up both chopsticks (as described) the philosopher will delay a
 	// 	number of nanoseconds that is determined by you experimentally.
	delay(100000);
	// After the delay completes

	//release left (since right was picked up first)
	pthread_mutex_lock(&(mutex[left]));
	chopsticks[left] = -1;
	pthread_mutex_unlock(&(mutex[left]));

	delay(9999);

	//release right
	pthread_mutex_lock(&(mutex[phil_id]));
	chopsticks[phil_id] = -1;
	pthread_mutex_unlock(&(mutex[phil_id]));


} // end eat function
