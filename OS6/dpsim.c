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

} // end th_main function


void* th_phil( void* th_phil_args ) {

	// ---------------------------------------
	// TODO: you add your implementation here



} // end th_phil function


void delay( long nanosec ) {

	struct timespec t_spec;

	t_spec.tv_sec = 0;
	t_spec.tv_nsec = nanosec;

	nanosleep( &t_spec, NULL );

} // end think function


void eat( int phil_id ) {

	// ---------------------------------------
	// TODO: you add your implementation here

} // end eat function
