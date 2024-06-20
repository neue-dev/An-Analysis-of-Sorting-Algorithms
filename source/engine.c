/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-11 00:20:03
 * @ Modified time: 2024-06-20 15:46:50
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
#define PARAM_N (1 << _PARAM + 0)
#define PARAM_P (1 << _PARAM + 1)
#define PARAM_ENTROPY (1 << _PARAM + 2)
#define PARAM_R (1 << _PARAM + 3)
#define PARAM_RSQUARED (1 << _PARAM + 4)
#define PARAM_CYCLES (1 << _PARAM + 5)
#define PARAM_COUNT 6

#define _SORTER PARAM_COUNT
#define SORTER_HEAP (1 << (_SORTER + 0))
#define SORTER_INSERTION (1 << (_SORTER + 1))
#define SORTER_MERGE (1 << (_SORTER + 2))
#define SORTER_SELECTION (1 << (_SORTER + 3))
#define SORTER_SMOOTH (1 << (_SORTER + 4))
#define SORTER_TIM (1 << (_SORTER + 5))
#define SORTER_COUNT 6

#define MAX_ROWS 32
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
 * This is something we call after every set of runs.
 * Ensure that we reset the value of run count.
 * 
 * @param   { Engine * }  this  The tester engine to initialize.
*/
void Engine_reset(Engine *this) {
  int i, j;
  
  // Reset run count
  this->runCount = 0;

  // Populate the array with 0s
  for(i = 0; i < MAX_RUNS; i++)
    for(j = 0; j < MAX_ROWS; j++)
      this->runs[j][i] = 0.0;

  // Populate the array with 0s too
  // We have to reset this here too
  for(i = 0; i < MAX_CYCLES; i++)
    for(j = 0; j < MAX_ROWS; j++)
      this->cycles[j][i] = 0.0;
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
    case SORTER_TIM:        return "tim sort";
    default:                return "?? sort";
  }
}

/**
 * Returns the frequency count used by the given sorter.
 * 
 * @param   { Engine * }  this    The Engine object.
 * @param   { int }       sorter  The int code for the sorter.
*/
long _Engine_getFreqCount(Engine *this, int sorter) {
  switch(sorter) {
    case SORTER_HEAP:       return Record_getHeapSortFreq();
    case SORTER_INSERTION:  return Record_getInsertionSortFreq();
    case SORTER_MERGE:      return Record_getMergeSortFreq();
    case SORTER_SELECTION:  return Record_getSelectionSortFreq();
    case SORTER_SMOOTH:     return Record_getSmoothSortFreq();
    case SORTER_TIM:        return Record_getTimSortFreq();
    default:                return -1;
  }
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
  printf("[#] Setting parameters for sorting:\n -  N=%d records with\n -  P=%.4lf amount of shuffling...\n\n", N, P);
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
  printf("[#] Data has been shuffled.\n -  Entropy=%.4lf;\n -  R=%.4lf;\n -  R^2=%.4lf;\n -  Is sorted=%s.\n\n", 
    tester->entropy, tester->r, tester->rsquared, Tester_checkSort(tester) ? "yes" : "no");
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

    case SORTER_TIM:
      Record_timSort(tester->tosort, tester->N);
      break;

    default: break;
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

      // Save the time taken and the freq count
      this->cycles[j][i] = t;
      this->cycles[j + SORTER_COUNT][i] = _Engine_getFreqCount(this, sorter);

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
  int sorter;
  double t, f;

  // Grab the tester
  // We only need it for the entropy and rsquared value here
  Tester *tester = &this->tester;

  // Stores our stats
  double means[MAX_ROWS];
  double bests[MAX_ROWS];
  double worsts[MAX_ROWS];

  // Compute the means and best runs
  for(i = 0; i < SORTER_COUNT; i++) {
    
    // Time
    means[i] = 0;
    bests[i] = (double) (1ULL << 40);
    worsts[i] = 0;

    // Freq counts
    means[i + SORTER_COUNT] = 0;
    bests[i + SORTER_COUNT] = (double) (1ULL << 40);
    worsts[i + SORTER_COUNT] = 0;

    for(j = 0; j < this->cycleCount; j++) {
      t = this->cycles[i][j];
      f = this->cycles[i + SORTER_COUNT][j];

      // Update the means
      means[i] += t;
      means[i + SORTER_COUNT] += f;

      // Update extremes
      if(t < bests[i]) bests[i] = t;
      if(t > worsts[i]) worsts[i] = t;
      if(f < bests[i + SORTER_COUNT]) bests[i + SORTER_COUNT] = f;
      if(f > worsts[i + SORTER_COUNT]) worsts[i + SORTER_COUNT] = f;
    }

    // Update the means
    means[i] /= this->cycleCount;
    means[i + SORTER_COUNT] /= this->cycleCount;
  }

  printf("\n[$] Cycles summary:\n");
  
  // Print the summary data
  for(i = 0; i < SORTER_COUNT; i++) {

    // Get the sorting algo
    sorter = 1 << (_SORTER + i);

    if(!(sorter & sorters))
      continue;

    // Print its info
    printf(" - %s ave time: %.2lf, best time: %.0lf, worst time: %.0lf.\n", 
      _Engine_getName(this, sorter), means[i], bests[i], worsts[i]);
    printf(" - %s ave freq: %.0lf, best freq: %.0lf, worst freq: %.0lf.\n", 
      _Engine_getName(this, sorter), means[i + SORTER_COUNT], bests[i + SORTER_COUNT], worsts[i + SORTER_COUNT]);
    
    // Save the summary to the runs
    this->runs[_SORTER + i][this->runCount] = means[i];
    this->runs[_SORTER + i + SORTER_COUNT][this->runCount] = means[i + SORTER_COUNT];
  }

  // Save the other info to the runs
  this->runs[0][this->runCount] = N;
  this->runs[1][this->runCount] = P;
  this->runs[2][this->runCount] = tester->entropy;
  this->runs[3][this->runCount] = tester->r;
  this->runs[4][this->runCount] = tester->rsquared;
  this->runs[5][this->runCount] = this->cycleCount;

  // Increment the runs count
  this->runCount++;
}

/**
 * Displays the summary of the current data in the run buffer.
*/
void _Engine_doRunsSummary(Engine *this) {
  
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
void Engine_doRun(Engine *this, int N, double P, int cycles, int sorters) {

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
void Engine_doRuns(Engine *this, int N, double P, int cycles, int runs, int sorters) {
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

    // Run once
    Engine_doRun(this, N, P, cycles, sorters);
  }
}

#endif

