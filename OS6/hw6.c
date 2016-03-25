#include <stdio.h>
#include "dpsim.h"
#include <pthread.h>

/**************************************************

Main Function:
	int main( int argc, char** argv )

------------------------------------------------
In this function perform the following steps:
------------------------------------------------
1. Create the following variables:
	- main_thread (pthread_t)
	- status (join status value)
2. Create a main_thread
	- If the return value != 0, then display an error message and
	  immediately exit program with status value 1.
3. Join the main_thread
	- If the return value != 0, then display an error message and
	  immediately exit the program with status value 2.
4. Display join status value.
5. Exit program.

*/

int main( int argc, char** argv ) {

	// 1. Create the following variables:
	// 	- main_thread (pthread_t)
	pthread_t main_thread;
	// 	- status (join status value)
	int status;
	// 2. Create a main_thread
	// 	- If the return value != 0, then display an error message and
	// 	  immediately exit program with status value 1.
	if (pthread_create(&main_thread, NULL, th_main, NULL) != 0)
	{
		//displaying error message
		fprintf(stderr, "failed to create main_thread, terminating\n");
		//exit with status value 1
		exit(1);
	}
	// 3. Join the main_thread
	// 	- If the return value != 0, then display an error message and
	// 	  immediately exit the program with status value 2.
	if ((status = pthread_join(main_thread, NULL)) != 0){
		//display error message
		fprintf(stderr, "failed to join main_thread terminating with error code: %d\n", status);
		//exit with status value 2
		exit(2);
	}
	// 4. Display join status value.
	printf("join status value: %d\n", status);
	// 5. Exit program.
	return 0;

} // end main function
