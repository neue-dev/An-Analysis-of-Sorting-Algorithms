/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-09 01:31:58
 * @ Modified time: 2024-06-15 14:56:14
 * @ Description:
 * 
 * An implementation of heap sort.
 */

#ifndef HEAP_SORT_C
#define HEAP_SORT_C

#include "./sorter.h"

/**
 * Holds information about the heapsort algorithm we'll be using.
 * The comparator tells heap sort how to compare two entries.
 * The swapper tells heap sort how to swap two entries.
*/
typedef struct HeapSort {

  t_Comparator comparator;
  t_Swapper swapper;
  t_Copier copier;
  t_Sizer sizer;
  
} HeapSort;

/**
 * Initializes the heap sort data object.
 * 
 * @param   { HeapSort * }    this        The heap sort data object to init.
 * @param   { t_Comparator }  comparator  The comparator to use for sorting.
 * @param   { t_Swapper }     swapper     The swapper to use for sorting.
 * @param   { t_Copier }      copier      The copier to use in case needed.
 * @param   { t_Sizer }       sizer       A function that returns the size of a record.
*/
void HeapSort_init(HeapSort *this, t_Comparator comparator, t_Swapper swapper, t_Copier copier, t_Sizer sizer) {
  this->comparator = comparator;
  this->swapper = swapper;
  this->copier = copier;
  this->sizer = sizer;
}

/**
 * This function converts a subset of the given array into a valid max-heap.
 * The heapified part begins at the index of the provided root.
 * Note that we call this function repeatedly beginning with the leaves of the heap (array).
 * 
 * @param   { HeapSort }      this        The heap sort "object" we're using.
 * @param   { t_Record }      records     The array of records, a subset of which we will heapify.
 * @param   { t_Record }      rRoot       A copy of the root element we're shifting down.
 * @param   { int }           n           The total number of records.
 * @param   { int }           root        The index of the root node to start with.
*/
void _HeapSort_heapify(HeapSort this, t_Record records, int n, t_Record rRoot, int root) {

  // The index that tracks the largest element
  int largest = root;

  // Indices of the children
  int l = root * 2 + 1;
  int r = root * 2 + 2;

  // r is the only possible larger one
  if(l >= n && r < n)
    if(this.comparator(records, rRoot, r, 0) > 0)
      largest = r;

  // l is the only possible larger one
  if(r >= n && l < n)
    if(this.comparator(records, rRoot, l, 0) > 0)
      largest = l;

  // If both may be larger, choose which one
  if(l < n && r < n) {

    // Get the larger one
    if(this.comparator(records, records, l, r) < 0)
      largest = r;
    else
      largest = l;
  }

  // Both elements are less than the root so just use the root
  if(this.comparator(records, rRoot, largest, 0) < 0)
    largest = root;

  // If the largest element is not at the root, then we swap to fix the heap
  if(largest != root) {

    // Swap the two elements
    this.copier(records, records, root, largest);
    
    // We heapify the subtree that we modified because of the swap
    // We only need to heapify that subtree because we're performing this operation starting from the leaves of the array
    // Hence, the other subtree should still be a valid max-heap because it was already heapified prior to this iteration
    _HeapSort_heapify(this, records, n, rRoot, largest);
  
  // Finally, put the copied root into its slot
  } else {
    this.copier(records, rRoot, root, 0);
  }
}

/**
 * This function converts the entire array of records into a valid max-heap.
 * Again, we start performing the heapify routine on the "leaves" of the array.
 * 
 * @param   { t_Record }  records   The records to sort.
 * @param   { int }       n         The number of records to sort.
*/
void _HeapSort_toHeap(HeapSort this, t_Record records, int n) {
  int i;

  // Temp holding var
  t_Record r = calloc(1, this.sizer());

  // Unconventional for loop lol
  // I just don't like writing (n - 1)
  for(i = n; --i >= 0;) {

    // Copy the current root
    this.copier(r, records, 0, i);

    // Heapify the current tree
    _HeapSort_heapify(this, records, n, r, i);
  }

  // Free the memory
  free(r);
}

/**
 * Now I know I chose smooth sort as my sorting algorithm of choice, but
 * the design of heap sort is what helps one understand smooth sort better.
 * By implementing heap sort first, I feel like it should give me a better
 * understanding of smoort sort eventually.
 * 
 * This function performs heap sort on the given array.
 * 
 * @param   { HeapSort }  this      The heap sort config object.
 * @param   { t_Record }  records   The array of records to sort.
 * @param   { int }       n         The number of records in the array.
*/
void HeapSort_main(HeapSort this, t_Record records, int n) {
  int i;

  // Temp holding var
  t_Record r = calloc(1, this.sizer());

  // Convert the array into a valid max-heap first
  _HeapSort_toHeap(this, records, n);
  
  // After that, start from the last element and swap the max with it each time
  // Following each swap, we fix the max-heap using a sift down operation
  for(i = n; --i >= 0;) {

    // Swap first element with last
    this.swapper(records, 0, i);

    // Grab a copy of the root
    this.copier(r, records, 0, 0);

    // Do the sift down on n - 1
    if(i - 1)
      _HeapSort_heapify(this, records, i, r, 0);
  }

  // Free memory
  free(r);
}

#endif