#include "mem_impl.h"
#include "mem.h"
#include <time.h>

#define NTRIALS 10000
#define PCTGET 50
#define PCTLARGE 10
#define SMALL_LIMIT 200
#define LARGE_LIMIT 20000

void printreport(clock_t time);

int main(int argc, char** argv) {

  unsigned int ntrials     = argc>1 ? atoi(argv[1]) : NTRIALS;// initialize all
  unsigned int pctget      = argc>2 ? atoi(argv[2]) : PCTGET; // but rand_seed
  unsigned int pctlarge    = argc>3 ? atoi(argv[3]) : PCTLARGE;
  unsigned int small_limit = argc>4 ? atoi(argv[4]) : SMALL_LIMIT;
  unsigned int large_limit = argc>5 ? atoi(argv[5]) : LARGE_LIMIT;
  unsigned int random_seed;
  FILE* randsource = fopen("/dev/urandom", "r"); // default for rand() is 
  fread(&random_seed, sizeof(int), 1, randsource); // calculated, not loaded
  random_seed = argc>6 ? atoi(argv[6]) : random_seed; //arg overrides default
  srand(random_seed);
  
  
  void* alloc_list[ntrials]; // need a list of pointers to find a block in O(1)
  int which_block[ntrials]; // random alloc_list indices of blocks to free
  int block_size[ntrials]; // list of random block sizes to get
  int list_length = 0; // current number of items on alloc_list
  int k;
  clock_t time, total_time;
  
  // make all random choices in advance and record them in a test plan
  for(k=0; k < ntrials; k++) {
    if((rand() > RAND_MAX/100*pctget) && (list_length > 0)) { // free a block
      which_block[k] = rand()/(RAND_MAX/list_length--); //choose block to free
      block_size[k] = 0;
    }
    else { // get a block
      if(rand() > RAND_MAX/100*pctlarge) { // block is small
        block_size[k] = 1 + rand()/(RAND_MAX/(small_limit -1)); // choose size
      }
      else { // block is large
        block_size[k] = small_limit + rand()/(RAND_MAX/(large_limit - small_limit));
      }
      which_block[k] = 0; // should be unused if things are working
      list_length++;
    }
  }
  // execute the test plan to get a timing of our functions w/o much overhead
  list_length = 0; // reset the length of alloc_list or it crashes randomly!
  time = clock();
  total_time = 0;
  for(k=0; k < ntrials; k++) {
    if(block_size[k]) { // block_size != 0 means getmem was chosen at this k
      alloc_list[list_length++] = getmem(block_size[k]);
      }
    else { // freemem was chosen at this k
      freemem(alloc_list[which_block[k]]); // insert block into free list
      alloc_list[which_block[k]] = alloc_list[--list_length]; // plug hole in list
    }
    // true at 10% intervals including last.
    if((ntrials - k-1)%(ntrials/10) == 0){
      time = clock() - time;
      total_time = total_time + time;
      printreport(time);
      time = clock();
    }
  }
  printf("Total time =  %f\n", (float) total_time/CLOCKS_PER_SEC);
  return 0;
}

void printreport(clock_t time) {
  uintptr_t total_size = 0; // these three hold the output of get_mem_stats
  uintptr_t total_free = 0;
  uintptr_t n_free_blocks = 0;
  
  get_mem_stats(&total_size, &total_free, &n_free_blocks);
  FILE* heapfileptr =  fopen("heaplist.txt","w");
  print_heap(heapfileptr);
  fclose(heapfileptr);
  printf("Total mem: %" PRIuPTR "\t", total_size);
  printf("Free blocks: %" PRIuPTR "\t", n_free_blocks);
  if(n_free_blocks != 0) {
    printf("Mean size: %" PRIuPTR "\t", total_free/n_free_blocks);
  }
  else {
    printf("Undefined\t");
  }
  printf("Time: %f\n", (float) time/CLOCKS_PER_SEC);
}

