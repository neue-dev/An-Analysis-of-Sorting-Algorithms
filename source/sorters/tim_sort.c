/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-14 23:35:14
 * @ Modified time: 2024-06-15 02:50:24
 * @ Description:
 * 
 * An implementation of tim sort.
 */

#ifndef TIM_SORT_C
#define TIM_SORT_C

#include "./sorter.h"

/**
 * Holds information about the tim sort algorithm we'll be using.
 * The comparator tells tim sort how to compare two entries.
 * The swapper tells tim sort how to swap two entries.
*/
typedef struct TimSort {

  t_Comparator comparator;
  t_Swapper swapper;
  t_Copier copier;
  t_Sizer sizer;

  // The size of the runs we'll be dealing with
  int runSize;

} TimSort;

/**
 * Initializes the tim sort data object.
 * 
 * @param   { TimSort * }       this        The tim sort data object to init.
 * @param   { t_Comparator }    comparator  The comparator to use for sorting.
 * @param   { t_Swapper }       swapper     The swapper to use for sorting.
 * @param   { t_Copier }        copier      The copier to use in case needed.
 * @param   { t_Sizer }         sizer       A function that returns the size of a record.
*/
void TimSort_init(TimSort *this, t_Comparator comparator, t_Swapper swapper, t_Copier copier, t_Sizer sizer) {
  this->comparator = comparator;
  this->swapper = swapper;
  this->copier = copier;  
  this->sizer = sizer;

  // In the actual implementation, I think the run size is adaptive
  // In this case, to reduce implementation complexity, we set it to a fixed '16'
  this->runSize = 16;
}

/**
 * Copies the source array unto the records array.
 * We need this because tim sort, like merge sort, uses aux memory.
 * 
 * @param   { MergeSort }   this      The tim sort data object.
 * @param   { t_Record }    records   A list of the original records.
 * @param   { t_Record }    src       The merge-sorted records.
 * @param   { int }         n         The total number of records.
*/
void _TimSort_copySorted(TimSort this, t_Record records, t_Record src, int n) {
  int i;
  
  // Copy the individual records
  for(i = 0; i < n; i++) 
    this.copier(records, src, i, i);
}

/**
 * Merges two runs defined by tim sort.
 * 
 * @param   { TimSort * }   this      The tim sort data object.
 * @param   { t_Record }    records   The array of records.
 * @param   { t_Record }    target    The array of sorted records.
 * @param   { int }         n         The number of records.
 * @param   { int }         left      The leftmost part of the merge pair.
 * @param   { int }         right     The middle part of the merge pair.
 * @param   { int }         end       The rightmost part of the merge pair.
*/
void _TimSort_merge(TimSort this, t_Record records, t_Record target, int n, int left, int right, int end) {
  int i;
  int middle;

  // Clamp the values to n
  right = right > n ? n : right;
  end = end > n ? n : end;

  // Store the boundary between the two subarrays
  middle = right;

  // Perform the merging
  for(i = left; i < end; i++) {

    // The left boundary has hit the middle
    if(left >= middle) {
      this.copier(target, records, i, right++);
      continue;
    }

    // The right boundary has hit the end
    if(right >= end) { 
      this.copier(target, records, i, left++);
      continue;  
    }

    // Compare the left and right starting elements
    switch(this.comparator(records, left, right)) {
      
      case -1:  // The left comes first
        this.copier(target, records, i, left++);
        break;
      
      case 0:   // Default behaviour when theyre the same
      case 1:   // The right comes first
        this.copier(target, records, i, right++);
        break; 
    }
  }
}

/**
 * Performs tim sort on the given array of records.
 * 
 * @param   { TimSort * }   this      The tim sort data object.
 * @param   { t_Record }    records   An array of records.
 * @param   { int }         n         The size of the array.
*/
void TimSort_main(TimSort this, t_Record records, int n) {
  int i, j;

  // Allocate memory for the sorted array
  t_Record sorted = calloc(n, this.sizer());

  // Fix the runs
  for(i = 0; i < n; i++) {

    // Get the current index
    j = i;

    // Insertion sort for that run
    while(j % this.runSize && this.comparator(records, j - 1, j) > 0) {
      if(j < 0)
        break;
      
      this.swapper(records, j - 1, j);
      j--;
    }
  }

  // Go through successive increasing lengths
  for(i = this.runSize; i < n; i *= 2) {

    // Perform the merges
    for(j = 0; j < (n + i) / i; j += 2)
      _TimSort_merge(this, records, sorted, n, i * j, i * (j + 1), i * (j + 2));

    // Copy the sorted array unto the original
    _TimSort_copySorted(this, records, sorted, n);
  }

  // Free the allocated memory
  free(sorted);
}

#endif