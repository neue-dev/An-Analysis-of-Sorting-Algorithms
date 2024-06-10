/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-10 12:31:00
 * @ Modified time: 2024-06-10 13:50:25
 * @ Description:
 * 
 * The file contains all the testing utilities we will be using to benchmark our algorithms.
 * Thankfully, we can also check whether or not a list is sorted in O(n) time, and we do that here.
 * Additionally, this portion will include a custom testing rig that generates a shuffle of data 
 * and measures it's Shannon entropy and variance. The Shannon entropy helps us determine how 
 * "shuffled" our array is, while the variance can give us an idea of how "spread out" the values 
 * we're sorting are. We will also be using some of the provided starter data and will compute the
 * same parameters for those samples.
 */

#ifndef TESTER_C
#define TESTER_C

// We use merge sort to fix our records by default so we can compute their entropy
// Merge sort is chosen because it is the fastest stable sort we have in this project
#include "../sorters/merge_sort.c"
#include "../record.c"
#include "./tester.h"

#include <stdlib.h>

// We will sort a maximum of 1000000 records
#define MAX_RECORDS 1000000

// Holds the information we use while testing
// From hereon, all the functions will accept pointers to an instance of the Tester struct
// This is so we don't end up copying a million array entry pointers each time.
typedef struct Tester {

  t_Comparator comparator;          // Of course we still need this to check for the sortedness of a list
  t_Swapper swapper;                // We also need this because computing the entropy requires knowing 
                                    //    the sorted version of the list

  // 'Private' variables
  t_Record records[MAX_RECORDS];    // The records to sort
  int N;                            // The number of records to consider; 
                                    //    we won't always be sorting MAX_RECORDS

  double entropy;                   // The currently computed Shannon entropy
  double variance;                  // The currently computed variance

  // We need a temporary struct to wrap around the records so we can assign its index to the item
  struct t_RecordWrapper {

    t_Record record;  // The actual record instance
    int id;           // This is what helps us compute the Shannon entropy
    
  };

  // An array to temporarily store the wrapped records
  struct t_RecordWrapper _records[MAX_RECORDS];
  
} Tester;

/**
 * Initializes the specified tester object.
 * Note that this does not set the records to use nor does it set N.
 * The aforementioned action is delegated to a separate routine.
 * 
 * @param   { Tester * }      this        A pointer to the tester object itself.
 * @param   { t_Comparator }  comparator  The comparator to use.
 * @param   { t_Swapper }     swapper     The swapper to use.
*/
void Tester_init(Tester *this, t_Comparator comparator, t_Swapper swapper) {
  this->comparator = comparator;

  // Initially sets the entropy and variance to 0
  this->entropy = 0;
  this->variance = 0;
}

/**
 * Fills the records array with a sorted line of random records.
 * Each record has  a random id within 0-MAX_RECORDS. These ids may not be unique.
 * 
 * @param   { Tester * }  this  The tester data object.
*/
void Tester_recordsFill(Tester *this) {

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
 * Measures the entropy of a given shuffle of the data.
 * This function is automatically called by the tester after shuffling.
 * 
 * @param   { Tester * }  this  The pointer to the tester to use.
*/
double _Tester_measureEntropy(Tester *this) {
  int i;
  

  for(i = 0; i < this->N; i++) {

  }
}

/**
 * Measures the variance of a given shuffle of the data.
 * This function is automatically calld by the tester after sorting.
 * 
 * @param   { Tester * }  this  The pointer to the tester to use.
*/
double _Tester_measureVariance() {

}

/**
 * Checks whether the current list is actuall sorted.
 * We do this by checking whether the comparator returns the same value or 0 across all adjacent records.
 * In other words, the array is either strictly decreasing or strictly increasing.
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
    comparison = this->comparator(this->records, i, i + 1);
    
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