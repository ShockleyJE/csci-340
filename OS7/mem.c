#include <stdio.h>    /* for printf statements when debugging */
#include <stdlib.h>   /* for malloc() and free() */
#include "mem.h"

/*
  Physical memory array. This is a static global array for all
  functions in this file.  An element in the array with a value of
  zero represents a free unit of memory.
*/
static dur_t* memory;

/*
 The size (i.e. number of units) of the physical memory array. This is
 a static global variable used by functions in this file.
 */
static int mem_size;


/*
 The last_placement_position variable contains the end position of the
 last allocated unit used by the next fit placement algorithm.
 */
static int last_placement_position;

/*
  Using the memory placement algorithm, strategy, allocate size
  units of memory that will reside in memory for duration time units.

  If successful, this function returns the number of contiguous blocks
  (a block is a contiguous "chuck" of units) of free memory probed while
  searching for a suitable block of memory according to the placement
  strategy specified.  If unsuccessful, return -1.

  If a suitable contiguous block of memory is found, the first size
  units of this block must be set to the value, duration.
 */
int mem_allocate(mem_strategy_t strategy, unsigned int size, unsigned int duration){
	int result;

	switch (strategy){
		case FIRST:
			result = firstfit(size, duration);
			break;
		case NEXT:
			result = nextfit(size, duration);
			break;
		case BEST:
			result = bestfit(size, duration);
			break;
		default:
			fprintf(stderr, "strategy switch error\n");
			exit(1);
	}
	return result;
}

/*
  Go through all of memory and decrement all positive-valued entries.
  This simulates one unit of time having transpired.
 */
int mem_single_time_unit_transpired(){
	int i;
	for (i = 0; i < mem_size; i++){
		if (memory[i] > 0){
			memory[i]--;
		}
	}
	return 0;
}

/*
  Return the number of fragments in memory.  A fragment is a
  contiguous free block of memory of size less than or equal to
  frag_size.
 */
int mem_fragment_count(int frag_size){
	int current_size = 0, i, count = 0;
	for (i = 0; i < mem_size; i++){
		if (memory[i] == 0){
			current_size++;
		}
		else{
			if (current_size <= frag_size && current_size > 0){
				count++;
			}
			current_size = 0;
		}
	}
	return count;
}

/*
  Set the value of zero to all entries of memory.
 */
void mem_clear(){
	int i;
	for (i = 0; i < mem_size; i++){
		memory[i] = 0;
	}
	last_placement_position = 0;
}

/*
 Allocate physical memory to size. This function should
 only be called once near the beginning of your main function.
 */
 void mem_init(int size)
 {
   memory = malloc(sizeof(dur_t)*size);
   mem_size = size;
   last_placement_position = 0;
   mem_clear();
 }

/*
 Deallocate physical memory. This function should
 only be called once near the end of your main function.
 */
void mem_free(){
	free( memory );
}

/*
  Print memory for testing/debugging purposes.  This is an optional
  routine to write, but highly recommended!  You will need to test and
  debug your allocation algorithms.  Calling this routine in your
  main() after every allocation (successful or not) will help in this
  endeavor.  NOTE: you should print the memory contents in contiguous
  blocks, rather than single units; otherwise, the output will be very
  long.
 */
void print_mem(){
	int i;
	printf("memory: ");
	for (i = 0; i < mem_size; i++){
		printf("%d ", memory[i]);
	}
	printf("\n");
}

/*
	helper method to return the size of a chunk of memory at index start_index
*/
int getblock(int start_index){
	int i, current_size = 0;
	for (i = start_index; i < mem_size; i++){
		if (memory[i] == 0){
			current_size++;
		}
		else break;
	}
	return current_size;
}

/*
	helper method to return the index of the first empty position
*/
int getfirstempty(){
	int i;
	for (i = 0; i < mem_size; i++){
		if (memory[i] == 0){
			return i;
		}
	}
	return -1;
}
int firstfit(int size, int duration){
	int i, block_size, done = 0, tries = 0;
	i = getfirstempty();
	// no free memory
	if (i == -1){
		return -1;
	}
	while (i < mem_size && !done){
		block_size = getblock(i);
		if (block_size <= 0){
			i++;
		}
		else if (block_size >= size){
			// allocate it
			allocate(size, duration, i);
			done = 1;
		}
		else {
			tries += 1;
			i += block_size;
		}
	}
	if (done) {
		return tries;
	}
	else{
		return -1;
	}
}

int nextfit(int size, int duration){
	int i, block_size, done = 0, tries = 0, hitend = 0;
	i = last_placement_position;
	while (i < mem_size && !done){
		block_size = getblock(i);
		if (block_size <= 0){
			i++;
		}
		else if (block_size >= size){
			allocate(size, duration, i);
			last_placement_position = i;
			done = 1;
		}
		else {
			// probed a block of memory
			tries++;
			i += block_size;
			if (!hitend && i >= mem_size){
				// circled
				hitend = 1;
				i = i % mem_size;
			}
		}
	}
	if (done){
		return tries;
	} else {
		return -1;
	}
}

int bestfit(int size, int duration){
   int i, block_size, found = 0, best_index, best_size = mem_size + 1, tries = 0;
   i = getfirstempty();
   while (i < mem_size){
      block_size = getblock(i);
      if (block_size <= 0){
         i++;
      }
      else if (block_size == size){
         found = 1;
         return i;
      }
      else if (block_size > size && block_size < best_size){
         // chunk found can fit the block and
         // it is smaller than the next smallest block
         best_index = i;
         best_size = block_size;
			found = 1;
			i += block_size;
      }
      else {
         // block_size is smaller than size
			tries++;
			i += block_size;
      }
   }

	if (found){
		//success
		allocate(size, duration, best_index);
		return tries;
	}
	else{
		// failed to allocate memory on this pass
		return -1;
	}
}

void allocate(int size, int duration, int start_index){
	int x;
	for (x = 0; x < size; x++){
		memory[start_index + x] = duration;
	}
}
