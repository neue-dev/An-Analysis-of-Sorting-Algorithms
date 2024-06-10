/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-09 01:31:42
 * @ Modified time: 2024-06-10 18:39:38
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
}

/**
 * Performs insertion sort on the given array of records.
 * 
 * @param   { InsertionSort * }   this      The insertion sort config algorithm.
 * @param   { t_Record }          records   An array of records.
 * @param   { int }               n         The size of the array.
*/
void InsertionSort_main(InsertionSort this, t_Record records, int n) {
  int i, j;

  // Go through each of the elements
  for(i = 0; i < n - 1; i++) {
    j = i + 1;

    // Move the new location down the array until we hit the correct location
    while(this.comparator(records, j - 1, j) > 0) {

      // Shift the element down      
      this.swapper(records, j - 1, j);

      // If we've hit the end of the array, stop
      if(--j <= 0)
        break;
    }
  }
}

#endif