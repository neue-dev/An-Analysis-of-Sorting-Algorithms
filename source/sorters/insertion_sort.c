/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-09 01:31:42
 * @ Modified time: 2024-06-20 00:37:01
 * @ Description:
 * 
 * An implementation of insertion sort.
 */

#ifndef INSERTION_SORT_C
#define INSERTION_SORT_C

#include "./sorter.h"

/**
 * Holds information about the insertion sort algorithm we'll be using.
 * The comparator tells insertion sort how to compare two entries.
 * The swapper tells insertion sort how to swap two entries.
*/
typedef struct InsertionSort {

  t_Comparator comparator;
  t_Swapper swapper;
  t_Copier copier;
  t_Sizer sizer;

  long frequencyCount;

} InsertionSort;

/**
 * Initializes the insertion sort data object.
 * 
 * @param   { InsertionSort * }     this        The insertion sort data object to init.
 * @param   { t_Comparator }        comparator  The comparator to use for sorting.
 * @param   { t_Swapper }           swapper     The swapper to use for sorting.
 * @param   { t_Copier }            copier      The copier to use in case needed.
 * @param   { t_Sizer }             sizer       A function that returns the size of a record.
*/
void InsertionSort_init(InsertionSort *this, t_Comparator comparator, t_Swapper swapper, t_Copier copier, t_Sizer sizer) {
  this->comparator = comparator;
  this->swapper = swapper;
  this->copier = copier;  
  this->sizer = sizer;

  this->frequencyCount = 0;
}

/**
 * Performs insertion sort on the given array of records.
 * 
 * @param   { InsertionSort * }   this      The insertion sort config algorithm.
 * @param   { t_Record }          records   An array of records.
 * @param   { int }               n         The size of the array.
*/
void InsertionSort_main(InsertionSort *this, t_Record records, int n) {
  int i, j;

  // Temp holding location
  t_Record r = calloc(1, this->sizer());

  // Reset the frequency count
  this->frequencyCount = 0;

  // Go through each of the elements
  for(i = 0; i < n - 1; i++) {
    j = i + 1;

    // Increment the frequency count
    this->frequencyCount++;

    // Copy the current element into temp
    this->copier(r, records, 0, j);

    // Move the new location down the array until we hit the correct location
    while(this->comparator(records, r, j - 1, 0) > 0) {

      // Increment the frequency count
      this->frequencyCount++;

      // Shift the element down      
      this->copier(records, records, j, j - 1);

      // If we've hit the end of the array, stop
      if(--j <= 0)
        break;
    }

    // No need to swap
    if(j == i + 1)
      continue;

    // Place the element into its place
    this->copier(records, r, j, 0);
  }

  // Free the temp
  free(r);
}

#endif