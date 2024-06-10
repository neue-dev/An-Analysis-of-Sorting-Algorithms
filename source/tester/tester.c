/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-10 12:31:00
 * @ Modified time: 2024-06-10 20:16:09
 * @ Description:
 * 
 * The file contains all the testing utilities we will be using to benchmark our algorithms.
 * Thankfully, we can also check whether or not a list is sorted in O(n) time, and we do that here.
 * Additionally, this portion will include a custom testing rig that generates a shuffle of data 
 * and measures it's Shannon entropy and coefficient of determination (with respect to its sorted order). 
 * Both the Shannon entropy and determination coefficient (also called R squared) can help us determine 
 * a rough estimate of how "shuffled" our array is. We will also be using some of the provided starter 
 * data and will compute the same parameters for those samples.
 */

#ifndef TESTER_C
#define TESTER_C

// We use merge sort to fix our records by default so we can compute their entropy
// Merge sort is chosen because it is the fastest stable sort we have in this project
#include "../sorters/merge_sort.c"
#include "../utils/random.c"
#include "./tester.h"

#include <stdlib.h>

// We will sort a maximum of 1000000 records
#define MAX_RECORDS 1000000
#define WRAPPER_SIZE sizeof(t_RecordWrapper)

/**
 * We need a temporary struct to wrap around the records so we can assign its index to the item.
 * This is a wrapper 'class' around the record struct.
 * 
 * @class
*/
typedef struct t_RecordWrapper {

  t_Record record;                // The actual record instance
  int index;                      // This is what helps us compute the Shannon entropy + r squared
  
} t_RecordWrapper;

/**
 * Returns a pointer to the wrapped record stored at a given index in the records array.
 * A wrapped record contains a reference to the original record and a new field containing the order of that record in a sorted array.
 * 
 * @param   { t_RecordWrapper * }   dest      The destination array.
 * @param   { t_RecordWrapper * }   src       The source array.
 * @param   { int }                 i         The ith slot of dest.
 * @param   { int }                 j         The jth entry in src.
*/
void _Wrapper_copy(t_RecordWrapper *dest, t_RecordWrapper *src, int i, int j) {
  
  // Copy the record
  memcpy(
    dest + i * WRAPPER_SIZE, 
    src + j * WRAPPER_SIZE,
    WRAPPER_SIZE);
}

/**
 * Swaps two different wrappers at the given locations.
 * 
 * @param   { t_RecordWrapper * }   wrappers  The array of wrappers where the swap will occur.
 * @param   { int }                 i         The ith slot of dest.
 * @param   { int }                 j         The jth entry in src.
*/
void _Wrapper_swap(t_RecordWrapper *wrappers, int i, int j) {

  // Temp record
  t_RecordWrapper rw;

  // Swap the two
  _Wrapper_copy(&rw, wrappers, 0, i);
  _Wrapper_copy(wrappers, wrappers, i, j);
  _Wrapper_copy(wrappers, &rw, j, 0);
}

/**
 * This is our tester 'class'.
 * Holds the information we use while testing.
 * From hereon, all the functions will accept pointers to an instance of the Tester struct.
 * This is so we don't end up copying a million array entry pointers each time.
 * 
 * @class
*/
typedef struct Tester {

  MergeSort sorter;                 // A fast stable sorting algorithm which we ONLY USE to compute the entropy.
  t_Comparator comparator;          // Of course we still need this to check for the sortedness of a list
  t_Swapper swapper;                // We also need this because computing the entropy requires knowing the sorted version of the list
  t_Copier copier;                  // This copies records between two different locations
  t_Sizer sizer;                    // This returns the size of the record for us
  int wrapperSize;                  // Size of the wrapper struct

  // 'Private' variables
  t_Record tosort;                  // The records to sort
  t_Record shuffle;                 // A copy of the shuffled records so we can use the same shuffle multiple times (testing consistency)
  int N;                            // The number of records to consider; we won't always be sorting MAX_RECORDS

  double entropy;                   // The currently computed Shannon entropy
  double rsquared;                  // The currently computed coefficient of determination.
  int *histogram;                   // We need this for computing the entropy; to be explained more below

  // An array to temporarily store the wrapped records
  // We only need this for the shuffling process
  // So we can compute the entropy and r squared
  t_RecordWrapper *shuffleWrappers;
  
} Tester;

/**
 * Initializes the specified tester object.
 * Note that this does not set the records to use nor does it set N.
 * The aforementioned action is delegated to a separate routine.
 * 
 * @param   { Tester * }      this        A pointer to the tester object itself.
 * @param   { t_Comparator }  comparator  The comparator to use.
 * @param   { t_Swapper }     swapper     The swapper to use.
 * @param   { t_Copier }      copier      The copier function.
 * @param   { t_Sizer }       sizer       A function that returns the size of a record.
*/
void Tester_init(Tester *this, t_Comparator comparator, t_Swapper swapper, t_Copier copier, t_Sizer sizer) {
  int recordSize = sizer();
  int wrapperSize = sizeof(t_RecordWrapper);
  int intSize = sizeof(int);

  // Save the provided functions
  this->comparator = comparator;
  this->swapper = swapper;
  this->copier = copier;
  this->sizer = sizer;
  this->wrapperSize = wrapperSize;

  // We initialize the arrays using calloc()
  this->tosort = calloc(MAX_RECORDS, recordSize);
  this->shuffle = calloc(MAX_RECORDS, recordSize);
  this->shuffleWrappers = calloc(MAX_RECORDS, wrapperSize);
  this->histogram = calloc(MAX_RECORDS, intSize);

  // Initially sets the entropy and r squared to 0
  this->entropy = 0;
  this->rsquared = 0;

  // Configure the fast stable sorting algo to use
  MergeSort_init(&this->sorter, this->comparator, this->swapper, this->copier, this->sizer);
}

/**
 * This is just here so we can do some garbage collection.
 * 
 * @param   { Tester * }  this  The tester to exit.
*/
void Tester_exit(Tester *this) {
  free(this->tosort);
  free(this->shuffle);
  free(this->shuffleWrappers);
  free(this->histogram);
}

/**
 * Returns a pointer to the wrapped record stored at a given index in the records array.
 * A wrapped record contains a reference to the original record and a new field containing the order of that record in a sorted array.
 * 
 * @param   { Tester * }  this  The tester data object.
 * @param   { int }       i     The index of the requested wrapped record.
 * @return  { t_Record }        A pointer to the requested wrapped record.
*/
t_Record _Tester_wrappersGet(Tester *this, int i) {
  return this->shuffleWrappers + i * this->wrapperSize;
}

/**
 * Returns a pointer to the record stored at a given index in the records array.
 * 
 * @param   { Tester * }  this  The tester data object.
 * @param   { int }       i     The index of the requested record.
 * @return  { t_Record }        A pointer to the requested record.
*/
t_Record _Tester_recordsGet(Tester *this, int i) {
  return this->shuffle + i * this->sizer();
}

/**
 * Configures the data set so we can compute the entropy and r squared values after.
 * 
 * @param   { Tester * }  this  The tester data object.
*/
void _Tester_recordsConfig(Tester *this) {
  int i;

  // Sort the array so we can compute its entropy later
  MergeSort_main(this->sorter, this->shuffle, this->N);

  // Create a copy of the array with the wrapper struct
  for(i = 0; i < this->N; i++) {

    // Create a wrapper for the record
    t_RecordWrapper wrapper;

    // Save the index and reference to record
    wrapper.index = i;
    wrapper.record = _Tester_recordsGet(this, i);

    // Copy the wrapped record unto the wrapped shuffle 
    _Wrapper_copy(this->shuffleWrappers, &wrapper, i, 0);
  }
}

/**
 * Shuffles the array using the Fisher-Yates shuffle.
 * It's simpler than it sounds, trust me.
 * Note that this shuffles the 'shuffle' array, not the 'records' array.
 * 
 * @param   { Tester * }  this  The tester data object.
*/
void Tester_recordsShuffle(Tester *this) {
  int i;
  int swap;

  // Configure the 
  _Tester_recordsConfig(this);

  for(int i = 0; i < this->N; i++)
    printf("%d %d %d \n", i, ((t_RecordWrapper *)_Tester_wrappersGet(this, i))->index, ((Record *)((t_RecordWrapper *)_Tester_wrappersGet(this, i))->record)->idNumber);

  // Start from the back going forwards
  for(i = this->N; --i >= 0;) {

    // We do (i + 1) because the element must have a chance of swapping with itself
    swap = Random_generate(i);

    // Just do the swap if it doesn't swap with itself
    // We're swapping the wrappers here
    if(swap != i)
      _Wrapper_swap(this->shuffleWrappers, i, swap); 
  }

  for(int i = 0; i < this->N; i++)
    printf("%d %d %d \n", i, ((t_RecordWrapper *)_Tester_wrappersGet(this, i))->index, ((Record *)((t_RecordWrapper *)_Tester_wrappersGet(this, i))->record)->idNumber);
 
  // ! compute the entropy and determination
  // ! copy the shuffled array into "this->shuffle"
  // ! copy the shuffled array into "this->records"
}

/**
 * Initializes the array of records using a given function.
 * It also computes their entropies and what not.
 * 
 * @param   { Tester * }  this    The tester data object.
 * @param   { t_Filler }  filler  A function that fills an array with data / records.  
*/
void Tester_recordsFill(Tester *this, t_Filler filler) {
  filler(this->shuffle, this->N, MAX_RECORDS * this->sizer());
}

/**
 * Reads the records from a provided file.
 * It also computes their entropies and what not.
 * 
 * @param   { Tester * }  this  The tester data object.
 * @param   { char[] }    file  The path to the file to read.
*/
void Tester_recordsRead(Tester *this, char file[]) {
  
}

/**
 * Sets the current value of N for the tester.
 * 
 * @param   { Tester * }  this  The tester data object.
 * @param   { int }       N     The number of object to sort.
*/
void Tester_setN(Tester *this, int N) {

  // Set N to the value
  this->N = N;

  // We cant exceed the limit though
  if(N > MAX_RECORDS)
    this->N = MAX_RECORDS;
}

/**
 * Measures the entropy of a given shuffle of the data.
 * This function is automatically called by the tester after shuffling.
 * // ! to do measure this
 * 
 * @param   { Tester * }  this  The pointer to the tester to use.
*/
double _Tester_measureEntropy(Tester *this) {
  int i;
  

  for(i = 0; i < this->N; i++) {

  }
}

/**
 * Measures the R squared of a given shuffle of the data to its sorted order.
 * This function is automatically calld by the tester after sorting.
 * 
 * @param   { Tester * }  this  The pointer to the tester to use.
*/
double _Tester_measureRSquared(Tester *this) {

}

/**
 * Checks whether the current list is actuall sorted.
 * We do this by checking whether the comparator returns the same value or 0 across all adjacent records.
 * In other words, the array is either strictly decreasing or strictly increasing.
 * // ! please test if this is actually workin
 * 
 * @param   { Tester * }  this  A pointer to the tester data object to use.
 * @return  { int }             A boolean that indicates whether or not the list is sorted.
*/
int Tester_checkSort(Tester *this) {
  int i;
  int comparison = 0;
  int order = 0;

  // Iterate through every pair of adjacent items
  for(i = 0; i < this->N - 1; i++) {

    // Get the result of the comparison
    comparison = this->comparator(this->tosort, i, i + 1);
    
    // If the order has not been determined, set it
    if(!order)
      order = comparison;

    // It's okay for two adjacent elements to be equal
    // We just keep checking other elements if this is true
    if(!comparison)
      continue;

    // If they are not equal, however, they must be the same as the determined order
    // Otherwise, we have an instance of a flip in the list--it reverses sort order midway
    if(order != comparison)
      return 0;
  }

  // All checks were passed, so list is sorted
  return 1;
}

#endif