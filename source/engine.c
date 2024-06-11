/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-11 00:20:03
 * @ Modified time: 2024-06-11 11:50:45
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
#define PARAM_ENTROPY (1 << _PARAM  + 0)
#define PARAM_RSQUARED (1 << _PARAM + 1)
#define PARAM_SIZE (1 << _PARAM + 2)
#define PARAM_COUNT 3

#define _SORTER PARAM_COUNT
#define SORTER_HEAP (1 << (_SORTER + 0))
#define SORTER_INSERTION (1 << (_SORTER + 1))
#define SORTER_MERGE (1 << (_SORTER + 2))
#define SORTER_SELECTION (1 << (_SORTER + 3))
#define SORTER_SMOOTH (1 << (_SORTER + 4))
#define SORTER_COUNT 5

#define MAX_ROWS 16
#define MAX_RUNS 1024
#define MAX_CYCLES 256

/**
 * This struct stores statistics about 
*/
typedef struct Engine {
  
  // The tester object the engine will use
  Tester tester;

  // Stores all our data
  double runs[MAX_ROWS][MAX_RUNS];
  double cycles[MAX_ROWS][MAX_CYCLES];

  // Current number of runs we've done
  int runCount;
  int cycleCount;

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

  // Populate the array with 0s too
  // We use this memory to compute averages
  for(i = 0; i < MAX_CYCLES; i++)
    for(j = 0; j < MAX_ROWS; j++)
      this->cycles[j][i] = 0.0;
      
  // Set run and cycle count to 0
  this->runCount = 0;
  this->cycleCount = 0;

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
 * Sets the parameters for the sorting benchmark run.
 * 
 * @param   { Engine * }        this      The tester engine to run.
 * @param   { int }             N         How many records we wanna sort.
 * @param   { double }          P         Probability of swapping during shuffling stage.
*/
void _Engine_setParams(Engine *this, int N, double P) {
    
  // Grab the tester
  Tester *tester = &this->tester;

  // Set the params first
  printf("[#] Setting parameters for sorting:\n -  N=%d records with\n -  P=%.2lf amount of shuffling...\n\n", N, P);
  Tester_setN(tester, N);
  Tester_setP(tester, P);
}

/**
 * Populates the tosort array of the tester struct with records.
 * 
 * @param   { Engine * }        this      The tester engine to run.
*/
void _Engine_setRecords(Engine *this) {

  // Grab the tester
  Tester *tester = &this->tester;

  // Fill the tester with data then configure it
  // Print the computed entropy and rsquared
  Tester_recordsFill(tester, &Record_fill);
  Tester_recordsShuffle(tester);
  printf("[#] Data has been shuffled.\n -  Entropy=%.2lf;\n -  R^2=%.2lf;\n -  Is sorted=%s.\n\n", 
    tester->entropy, tester->rsquared, Tester_checkSort(tester) ? "yes" : "no");
}

/**
 * Do the sorting algorithm associated with the given number.
 * 
 * @param   { Engine * }  this    The Engine object.
 * @param   { int }       sorter  The int code for the sorter.
*/
void _Engine_doSort(Engine *this, int sorter) {
  
  // Grab the tester
  Tester *tester = &this->tester;

  // Execute the right algorithm
  switch(sorter) {

    case SORTER_HEAP:
      Record_heapSort(tester->tosort, tester->N);
      break;
    
    case SORTER_INSERTION:
      Record_insertionSort(tester->tosort, tester->N);
      break;
    
    case SORTER_MERGE:
      Record_mergeSort(tester->tosort, tester->N);
      break;
    
    case SORTER_SELECTION:
      Record_selectionSort(tester->tosort, tester->N);
      break;
    
    case SORTER_SMOOTH:
      Record_smoothSort(tester->tosort, tester->N);
      break;

    default: break;
  }
}

/**
 * Returns the right names for each of the sorting algorithms.
 * 
 * @param   { Engine * }  this    The Engine object.
 * @param   { int }       sorter  The int code for the sorter.
*/
char *_Engine_getName(Engine *this, int sorter) {
  switch(sorter) {
    case SORTER_HEAP:       return "heap sort";
    case SORTER_INSERTION:  return "insertion sort";
    case SORTER_MERGE:      return "merge sort";
    case SORTER_SELECTION:  return "selection sort";
    case SORTER_SMOOTH:     return "smooth sort";
    default:                return "?? sort";
  }
}

/**
 * Performs the cycles of a single run.
 * 
 * @param   { Engine * }  this      The Engine data object.
 * @param   { int }       cycles    How many cycles to run for each algorithm.
 * @param   { int }       sorters   The sorters we want to consider.
*/
void _Engine_doCycles(Engine *this, int cycles, int sorters) {
  int i, j;
  int sorter, sorted;
  long start, t;

  // Grab the tester
  Tester *tester = &this->tester;

  // Reset count for cycles
  this->cycleCount = 0;

  // Do the benchmarking for all the sorters
  printf("[#] Performing benchmarks...\n");

  // Run a number of cycles so data is more robust
  for(i = 0; i < cycles; i++) {

    // Inform us which cycle we're in
    printf("\n[!] Running cycle #%d:\n", i + 1);
    
    // Run the algo for each of the sorters
    for(j = 0; j < SORTER_COUNT; j++) {

      // The sorter we're at
      sorter = 1 << (_SORTER + j);

      // Store 0 by default
      this->cycles[j][i] = 0;

      // We're excluding the algorithm
      if(!(sorter & sorters))
        continue;

      // Make sure to reshuffle the array
      Tester_recordsRepeatShuffle(tester);

      // Perform a single timed sort cycle
      start = currentTimeMillis();
      _Engine_doSort(this, sorter);
      t = currentTimeMillis() - start;
      
      // Check sort order
      sorted = Tester_checkSort(tester);

      // Save the time taken
      this->cycles[j][i] = t;

      // Output the time
      printf(" -  %s took %ldms, (%s) ...\n", 
        _Engine_getName(this, sorter), t, 
        sorted ? "sorted" : "failed");
    }

    // Increment the cycle count
    this->cycleCount++;
  }
}

/**
 * Computes and displays the summary of the current data in the cycle buffer.
 * It also saves these summary data into the runs buffer.
 * 
 * @param   { Engine * }  this      The Engine data object.
 * @param   { int }       N         The number of records that were sorted.
 * @param   { double P }  P         The shuffling factor. 
 * @param   { int }       sorters   The sorters to consider.
*/
void _Engine_doCyclesSummary(Engine *this, int N, double P, int sorters) {
  int i, j;
  int t, sorter;

  // Stores our stats
  double means[MAX_ROWS];
  int bests[MAX_ROWS];
  int worsts[MAX_ROWS];

  // Compute the means and best runs
  for(i = 0; i < SORTER_COUNT; i++) {
    means[i] = 0;
    bests[i] = 1 << 30;
    worsts[i] = 0;

    for(j = 0; j < this->cycleCount; j++) {
      t = this->cycles[i][j];

      // Update the means
      means[i] += t;

      // Update extremes
      if(t < bests[i]) bests[i] = t;
      if(t > worsts[i]) worsts[i] = t;
    }

    // Update the means
    means[i] /= this->cycleCount;
  }

  printf("\n[$] Cycle summary:\n");
  
  // Print the summary data
  for(i = 0; i < SORTER_COUNT; i++) {

    // Get the sorting algo
    sorter = 1 << (_SORTER + i);

    if(!(sorter & sorters))
      continue;

    // Print its info
    printf(" - %s ave: %.2lf, best: %d, worst: %d.\n", 
      _Engine_getName(this, sorter), means[i], bests[i], worsts[i]);
    
    // Save the summary to the runs
    this->runs[_SORTER + i][this->runCount] = means[i];
  }

  // Save the other info to the runs
  this->runs[0][this->runCount] = N;
  this->runs[1][this->runCount] = P;

  // Increment the runs count
  this->runCount++;
}

/**
 * Displays the summary of the current data in the run buffer.
*/
void _Engine_printRunsSummary(Engine *this) {
  
}

/**
 * Performs a single run of the benchmarking with all algorithms.
 * 
 * @param   { Engine * }        this      The tester engine to run.
 * @param   { int }             N         How many records we wanna sort.
 * @param   { double }          P         Probability of swapping during shuffling stage.
 * @param   { int }             cycles    How many cycles to run for each algorithm.
 * @param   { int }             sorters   The sorters we want to consider.
*/
void Engine_runOnce(Engine *this, int N, double P, int cycles, int sorters) {

  // Grab the tester
  Tester *tester = &this->tester;

  // Max cycles
  if(cycles >= MAX_CYCLES)
    cycles = MAX_CYCLES - 1;
  
  // Set the params and display it
  _Engine_setParams(this, N, P);

  // Populate records array of tester 
  _Engine_setRecords(this);
  
  // Run the cycles
  _Engine_doCycles(this, cycles, sorters);  

  // Print of the cycles of this run
  _Engine_doCyclesSummary(this, N, P, sorters);
}

/**
 * Performs a mutliple runs of the benchmarking with all algorithms.
 * 
 * @param   { Engine * }        this      The tester engine to run.
 * @param   { int }             N         How many records we wanna sort.
 * @param   { double }          P         Probability of swapping during shuffling stage.
 * @param   { int }             cycles    How many cycles to run for each algorithm.
 * @param   { int }             runs      How many runs to perform.
 * @param   { int }             sorters   The sorters we want to consider.
*/
void Engine_run(Engine *this, int N, double P, int cycles, int runs, int sorters) {
  int i;

  // Grab the tester
  Tester *tester = &this->tester;

  // Max runs
  if(runs >= MAX_RUNS)
    runs = MAX_RUNS - 1;

  // Do the runOnce function n times
  for(i = 0; i < runs; i++) {
    printf("\e[1;1H\e[2J");
    printf("\n[#] =========================");
    printf("\n[#] [        RUN %3d        ]", i + 1);
    printf("\n[#] =========================\n\n");

    Engine_runOnce(this, N, P, cycles, sorters);
  }
}

#endif

