/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-09 01:32:05
 * @ Modified time: 2024-06-10 18:39:12
 * @ Description:
 * 
 * Selection sort implementation.
 */

#ifndef SELECTION_SORT_C
#define SELECTION_SORT_C

#include "./sorter.h"

/**
 * Holds information about the selection sort algorithm we'll be using.
 * The comparator tells selection sort how to compare two entries.
 * The swapper tells selection sort how to swap two entries.
*/
typedef struct SelectionSort {
  
  t_Comparator comparator;
  t_Swapper swapper;
  t_Copier copier;
  t_Sizer sizer;
  
} SelectionSort;

/**
 * Initializes the selection sort data object.
 * 
 * @param   { SelectionSort * }     this        The selection sort data object to init.
 * @param   { t_Comparator }        comparator  The comparator to use for sorting.
 * @param   { t_Swapper }           swapper     The swapper to use for sorting.
 * @param   { t_Copier }            copier      The copier to use in case needed.
 * @param   { t_Sizer }             sizer       A function that returns the size of a record.
*/
void SelectionSort_init(SelectionSort *this, t_Comparator comparator, t_Swapper swapper, t_Copier copier, t_Sizer sizer) {
  this->comparator = comparator;
  this->swapper = swapper;
  this->copier = copier;
  this->sizer = sizer;
}

/**
 * Performs selection sort on the given array of records.
 * 
 * @param   { SelectionSort }   this      The config object for this sorter.
 * @param   { t_Record }        records   The array of records.
 * @param   { int }             n         The size of the array.
*/
void SelectionSort_main(SelectionSort this, t_Record records, int n) {
  int i, j;
  int minIndex;

  for(i = 0; i < n - 1; i++) {
    
    // Set the initial value of the minima
    minIndex = i;
    
    // Look for the smallest array element
    for(j = i + 1; j < n; j++) {
      if(this.comparator(records, minIndex, j) > 0)
        minIndex = j;
    }

    // Do a swap between the two
    if(minIndex != i)
      this.swapper(records, minIndex, i);
  }
}

#endif