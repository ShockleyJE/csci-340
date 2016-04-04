#include <stdio.h>    /* for printf statements when debugging */
#include <stdlib.h>   /* for malloc() and free() */
#include "mem.h"

/*
  Physical memory array. This is a static global array for all functions in this file.
  An element in the array with a value of zero represents a free unit of memory.
*/
static unsigned int* memory;

/*
 The size (i.e. number of units) of the physical memory array. This is a static global
 variable used by functions in this file.

 */
static unsigned int mem_size;


/*
 The last_placement_position variable contains the end position of the last
 allocated unit used by the next fit placement algorithm.
 */
static unsigned int last_placement_position;

void print_mem(){
	int i;
	printf("memory: ");
	for (i = 0; i < mem_size; i++){
		printf("%d ", memory[i]);
	}
	printf("\n");
}

int getchunk(int startindex){
	int i, cursize = 0;
	for (i = startindex; i < mem_size; i++){
		if (memory[i] == 0){
			cursize++;
		}
		else break;
	}
	return cursize;
}

int getfirstempty(){
	int i;
	for (i = 0; i < mem_size; i++){
		if (memory[i] == 0){
			return i;
		}
	}
	return -1;
}

void allocate(int size, int duration, int startindex){
	int x;
	for (x = 0; x < size; x++){
		memory[startindex + x] = duration;
	}
}

int firstfit(int size, int duration){
	int i, chunksize, done = 0, tries = 0;
	i = getfirstempty();
	if (i == -1){
		// no free slots, return -1 for failure to allocate
		return -1;
	}
	while (i < mem_size && !done){
		chunksize = getchunk(i);
		if (chunksize <= 0){
			i++;
			//return -1;
		}
		else if (chunksize >= size){
			// allocate it
			allocate(size, duration, i);
			done = 1;
		}
		else {
			tries += 1;
			i += chunksize;
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
	int i, chunksize, done = 0, tries = 0, hitend = 0;
	i = last_placement_position;
	while (i < mem_size && !done){
		chunksize = getchunk(i);
		if (chunksize <= 0){
			i++;
		}
		else if (chunksize >= size){
			allocate(size, duration, i);
			last_placement_position = i;
			done = 1;
		}
		else {
			tries++;
			i += chunksize;
			if (!hitend && i >= mem_size){
				// wrap around once
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
   int i, chunksize, found = 0, bestindex, bestsize = mem_size + 1, tries = 0;
   i = getfirstempty();
   while (i < mem_size){
      chunksize = getchunk(i);
      if (chunksize <= 0){
         i++;
      }
      else if (chunksize == size){
         found = 1;
         return i;
      }
      else if (chunksize > size && chunksize < bestsize){
         bestindex = i;
         bestsize = chunksize;
			found = 1;
			i += chunksize;
      }
      else {
			tries++;
			i += chunksize;
      }
   }

	if (found){
		allocate(size, duration, bestindex);
		return tries;
	}
	else{
		return -1;
	}
}
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
	return 0; // ???
}

/*
  Return the number of fragments in memory.  A fragment is a
  contiguous free block of memory of size less than or equal to
  frag_size.
 */
int mem_fragment_count(int frag_size){
	int cursize = 0, i, count = 0;
	for (i = 0; i < mem_size; i++){
		if (memory[i] == 0){
			// still in a chunk
			cursize++;
		}
		else{
			if (cursize <= frag_size && cursize > 0){
				count++;
			}
			cursize = 0;
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
void mem_init( unsigned int size )
{
	memory = malloc( sizeof(unsigned int)*size );
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
