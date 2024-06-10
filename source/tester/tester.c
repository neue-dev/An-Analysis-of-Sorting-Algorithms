/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-10 12:31:00
 * @ Modified time: 2024-06-10 13:00:40
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

#include "../record.c"
#include "./tester.h"

// We will sort a maximum of 1000000 records
#define MAX_RECORDS 1000000

// Holds the information we use while testing
// From hereon, all the functions will accept pointers to an instance of the Tester struct
// This is so we don't end up copying a million array entry pointers each time.
typedef struct Tester {

  t_Comparator comparator;          // Of course we still need this to check for the sortedness of a list

  // 'Private' variables
  t_Record records[MAX_RECORDS];    // The records to sort
  int N;                            // The number of records to consider; 
                                    //    we won't always be sorting MAX_RECORDS

} Tester;

/**
 * Checks whether the current list is actuall sorted.
 * We do this by checking whether the comparator returns the same value or 0 across all adjacent records.
 * In other words, the array is either strictly decreasing or strictly increasing.
 * 
 * @param   { Tester * }  this  The tester data object to use.
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