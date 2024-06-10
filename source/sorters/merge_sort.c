/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-09 01:31:50
 * @ Modified time: 2024-06-10 18:45:14
 * @ Description:
 * 
 * An implementation of merge sort.
 */

#ifndef MERGE_SORT_C
#define MERGE_SORT_C

#include "./sorter.h"

/**
 * Holds information about the merge sort algorithm we'll be using.
 * The comparator tells merge sort how to compare two entries.
 * The swapper tells merge sort how to swap two entries.
*/
typedef struct MergeSort {
  
  t_Comparator comparator;
  t_Swapper swapper;
  t_Copier copier;
  t_Sizer sizer;

} MergeSort;

/**
 * Initializes a merge sort data object.
 * 
 * @param   { MergeSort * }   this        The merge sort object to init.
 * @param   { t_Comparator }  comparator  The comparator to use for sorting.
 * @param   { t_Swapper }     swapper     The swapper to use for sorting.
 * @param   { t_Copier }      copier      The copier to use in case needed.
 * @param   { t_Sizer }       sizer       A function that returns the size of a record.
*/
void MergeSort_init(MergeSort *this, t_Comparator comparator, t_Swapper swapper, t_Copier copier, t_Sizer sizer) {
  this->comparator = comparator;
  this->swapper = swapper;
  this->copier = copier;
  this->sizer = sizer;
} 

// ! remove
// /**
//  * Places the jth entry of src into the ith slot of dest.
//  * 
//  * @param   { MergeSort }   this      The merge sort config object.
//  * @param   { t_Record }    records   The records array.
//  * @param   { t_Record }    src       The source array.
//  * @param   { int }         i         The ith slot of records.
//  * @param   { int }         j         The jth entry in src.
// */
// void _MergeSort_copy(MergeSort this, t_Record dest, t_Record src, int i, int j) {

//   // Copy the record
//   // memcpy(
//   //   dest + i * this.recordSize, 
//   //   src + j * this.recordSize,
//   //   this.recordSize);
// }

/**
 * Copies the source array unto the records array.
 * We need this because our merge sort implementation uses aux memory.
 * 
 * @param   { MergeSort }   this      The merge sort config object.
 * @param   { t_Record }    records   A list of the original records.
 * @param   { t_Record }    src       The merge-sorted records.
 * @param   { int }         n         The total number of records.
*/
void _MergeSort_copySorted(MergeSort this, t_Record records, t_Record src, int n) {
  int i;
  
  // Copy the individual records
  for(i = 0; i < n; i++) 
    this.copier(records, src, i, i);
}

/**
 * This is an internal function to be used within the main merge sort routine.
 * 
 * @param   { MergeSort }   this      The merge sort config object
 * @param   { t_Record }    records   The array of records to sort.
 * @param   { t_Record }    target    The target array for the sorted elements.
 * @param   { int }         left      The start index of the left subarray.
 * @param   { int }         right     The start index of the right subarray.
 * @param   { int }         end       The largest index within both subarrays (the rightmost end of their union).
*/
void _MergeSort_merge(MergeSort this, t_Record records, t_Record target, int left, int right, int end) {
  int i;
  int middle;

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
 * Performs merge sort on the given set of records.
 * 
 * @param   { MergeSort }   this      The merge sort config object.
 * @param   { t_Record }    records   The array of records to sort.
 * @param   { int }         n         The number of records in the array.
*/
void MergeSort_main(MergeSort this, t_Record records, int n) {
  int i, j;

  // Left subarray start index,
  // Right subarray start index,
  // The end index of both subarrays
  int l, r, e;

  // Allocate memory for the sorted array
  t_Record sorted = calloc(n, this.sizer());

  // For each length of subarray (starting from 1 element)
  for(i = 1; i < n; i *= 2) {
    
    // For each pair of adjacent subarrays, we merge them together
    for(j = 0; j < n; j += i * 2) {

      // Note that i represents the current width we're using
      l = j;
      r = l + i < n ? l + i : n;
      e = l + i * 2 < n ? l + i * 2 : n;

      // Merge the two subarrays
      _MergeSort_merge(this, records, sorted, l, r, e);
    }

    // Copy the sorted array unto the original
    _MergeSort_copySorted(this, records, sorted, n);
  }

  // Free the allocated memory
  free(sorted);
}

#endif