/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-11 00:20:03
 * @ Modified time: 2024-06-11 01:21:44
 * @ Description:
 * 
 * Handles the overall flow of the program.
 * Also handles all the printing and displaying of information.
 */

#ifndef TESTER_ENGINE_C
#define TESTER_ENGINE_C

#include "./tester/tester.c"
#include "./utils/timer.c"
#include "./utils/sortingalgorithms.c"
#include "./utils/record_interface.c"

// Because I didn't want to port my old code for a hashmap, we're using a 2d array to store everything
// The first four rows will contain information about the data set to be sorted
// The next n rows will contain the times it took each sorting algorithm to sort the data set
#define _PARAM 0
#define PARAM_ENTROPY (1 << _PARAM)
#define PARAM_RSQUARED (2 << _PARAM)
#define PARAM_SIZE (3 << _PARAM)
#define PARAM_COUNT 3

#define _SORTER 2
#define SORTER_HEAP (1 << _SORTER)
#define SORTER_INSERTION (2 << _SORTER)
#define SORTER_MERGE (3 << _SORTER)
#define SORTER_SELECTION (4 << _SORTER)
#define SORTER_SMOOTH (5 << _SORTER)
#define SORTER_COUNT 5

#define MAX_ROWS 16
#define MAX_RUNS 1024

/**
 * This struct stores statistics about 
*/
typedef struct Engine {
  
  // The tester object the engine will use
  Tester tester;

  // Stores all our data
  double runs[MAX_RUNS][MAX_ROWS];

  // Current number of runs we've done
  int runCount;

} Engine;

/**
 * Initializes the tester engine.
 * 
 * @param   { Engine * }  this  The tester engine to initialize.
*/
void Engine_init(Engine *this) {
  int i, j;

  // Grab the tester
  Tester *tester = &this->tester;
  
  // Populate the array with 0s
  for(i = 0; i < MAX_RUNS; i++)
    for(j = 0; j < MAX_ROWS; j++)
      this->runs[j][i] = 0.0;
      
  // Set run count to 0
  this->runCount = 0;

  // Initialize the tester
  Tester_init(tester, &Record_comparator, &Record_swapper, &Record_copier, &Record_sizer);
}

/**
 * Does any garbage collection needed by the tester engine.
 * 
 * @param   { Engine * }  this  The tester engine to clean up.
*/
void Engine_exit(Engine *this) {
  
  // Grab the tester
  Tester *tester = &this->tester;
  
  // Perform some cleanup after
  Tester_exit(tester);
}

/**
 * Performs a single run of the benchmarking with all algorithms.
 * 
 * @param   { Engine * }  this  The tester engine to run.
 * @param   { int }             N     How many records we wanna sort.
 * @param   { double }          P     Probability of swapping during shuffling stage.
*/
void Engine_runOnce(Engine *this, int N, double P) {
  int i;
  long start, stop;
  
  // Grab the tester
  Tester *tester = &this->tester;
  
  // Set the params first
  printf("Initializing array for sorting N=%d records with shuffle amount P=%lf...\n\n", N, P);
  Tester_setN(tester, N);
  Tester_setP(tester, P);

  // Fill the tester with data then configure it
  // Print the computed entropy and rsquared
  Tester_recordsFill(tester, &Record_fill);
  Tester_recordsShuffle(tester);
  printf("Data has been shuffled.\n[] Entropy=%lf;\n[] R^2=%lf;\n[] Is sorted=%s.\n\n", 
    tester->entropy, tester->rsquared, Tester_checkSort(tester) ? "yes" : "no");
  
  // Do the benchmarking for all the sorters
  printf("Performing benchmarks...\n");

  // Run the algo for each of the sorters
  for(i = 0; i < SORTER_COUNT; i++) {

    // Make sure to reshuffle the array
    Tester_recordsRepeatShuffle(tester);

    // Start the timer
    start = currentTimeMillis();
    
    switch((i + 1) << _SORTER) {
      case SORTER_HEAP:
        Record_heapSort(tester->tosort, tester->N);
        stop = currentTimeMillis();
        printf("Heap sort took %ldms...\n", stop - start);
        break;
      case SORTER_INSERTION:
        Record_insertionSort(tester->tosort, tester->N);
        stop = currentTimeMillis();
        printf("Insertion sort took %ldms...\n", stop - start);
        break;
      case SORTER_MERGE:
        Record_mergeSort(tester->tosort, tester->N);
        stop = currentTimeMillis();
        printf("Merge sort took %ldms...\n", stop - start);
        break;
      case SORTER_SELECTION:
        Record_selectionSort(tester->tosort, tester->N);
        stop = currentTimeMillis();
        printf("Selection sort took %ldms...\n", stop - start);
        break;
      case SORTER_SMOOTH:
        Record_smoothSort(tester->tosort, tester->N);
        stop = currentTimeMillis();
        printf("Smooth sort took %ldms...\n", stop - start);
        break;
      default: break;
    }
  }

}

#endif

