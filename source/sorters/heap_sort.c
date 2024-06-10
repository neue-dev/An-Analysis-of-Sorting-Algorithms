/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-09 01:31:58
 * @ Modified time: 2024-06-10 02:44:59
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
  int recordSize;
  
} HeapSort;

/**
 * Initializes the heap sort data object.
 * 
 * @param   { HeapSort * }    this        The heap sort data object to init.
 * @param   { t_Comparator }  comparator  The comparator to use for sorting.
 * @param   { t_Swapper }     swapper     The swapper to use for sorting.
 * @param   { int }           recordSize  The size (in bytes) of a single record.
*/
void HeapSort_init(HeapSort *this, t_Comparator comparator, t_Swapper swapper, int recordSize) {
  this->comparator = comparator;
  this->swapper = swapper;
  this->recordSize = recordSize;
}

/**
 * This function converts a subset of the given array into a valid max-heap.
 * The heapified part begins at the index of the provided root.
 * Note that we call this function repeatedly beginning with the leaves of the heap (array).
 * 
 * @param   { HeapSort }      this        The heap sort "object" we're using.
 * @param   { t_Record }      records     The array of records, a subset of which we will heapify.
 * @param   { int }           n           The total number of records.
 * @param   { int }           root        The index of the root node to start with.
*/
void _HeapSort_heapify(HeapSort this, t_Record records, int n, int root) {
  
  // The index that tracks the largest element
  int largest = root;

  // Indices of the children
  int l = root * 2 + 1;
  int r = root * 2 + 2;

  // These two statements tell us which among the three (root, left child, right child)
  // is the largest element
  if(l < n)
    if(this.comparator(records, l, largest) > 0)
      largest = l;

  if(r < n)
    if(this.comparator(records, r, largest) > 0)
      largest = r;

  // If the largest element is not at the root, then we swap to fix the heap
  if(largest != root) {

    // Swap the two elements
    this.swapper(records, root, largest);
    
    // We heapify the subtree that we modified because of the swap
    // We only need to heapify that subtree because we're performing this operation starting from the leaves of the array
    // Hence, the other subtree should still be a valid max-heap because it was already heapified prior to this iteration
    _HeapSort_heapify(this, records, n, largest);
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

  // Unconventional for loop lol
  // I just don't like writing (n - 1)
  for(i = n; --i >= 0;)
    _HeapSort_heapify(this, records, n, i);
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

  // Convert the array into a valid max-heap first
  _HeapSort_toHeap(this, records, n);
  
  // After that, start from the last element and swap the max with it each time
  // Following each swap, we fix the max-heap using a sift down operation
  for(i = n; --i >= 0;) {

    // Swap first element with last
    this.swapper(records, 0, i);

    // Do the sift down on n - 1
    if(i - 1)
      _HeapSort_heapify(this, records, i, 0);
  }
}

#endif