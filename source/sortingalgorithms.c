/**
 * @ Author: Mo David
 * @ Create Time: 2024-05-24 11:21:27
 * @ Modified time: 2024-06-10 03:52:57
 * @ Description:
 * 
 * The file contains a wrapper for each of the implementations of the sorting algorithms.
 */

#ifndef SORTINGALGORITHMS_C
#define SORTINGALGORITHMS_C

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "record.c"

// Import the different sorting algorithms
#include "./sorters/insertion_sort.c"
#include "./sorters/selection_sort.c"
#include "./sorters/merge_sort.c"
#include "./sorters/heap_sort.c"
#include "./sorters/smooth_sort.c"
#include "./sorters/momo_sort.c"

/**
 * #####                            #######
 * #####                            #######
 * #####  Record Helper Functions   #######
 * #####                            #######
 * #####                            #######
*/

/**
 * This function compares two records.
 * We can swap this out for another function if we wish to redefine how we compare two records.
 * 
 * @param   { Record }  r1  The first record.
 * @param   { Record }  r2  The second record.
 * @return  { int }         -1 means the r1 < r2; 1 means r1 > r2; 0 means r1 = r2.
*/
int RecordArray_comparator(Record *records, int i, int j) {
  if(records[i].idNumber < records[j].idNumber)
    return -1;

  if(records[i].idNumber > records[j].idNumber)
    return 1;

  // The two have equal id numbers
  return 0;
}

/**
 * Swaps two records within the array.
 * 
 * @param   { Record * }  records   The array of records.
 * @param   { int }       i         The index of the first record.
 * @param   { int }       j         The index of the second record.
*/
void RecordArray_swapper(Record *records, int i, int j) {
  
  // Temp record
  Record rt;

  // Swap the two
  rt = records[i];
  records[i] = records[j];
  records[j] = rt;
}

/**
 * #####                      #######
 * #####                      #######
 * #####  Sorting Functions   #######
 * #####                      #######
 * #####                      #######
*/

/**
 * Insertion sort.
 * 
 * @param   { Record * }  records   The records to sort.
 * @param   { int }       n         The number of records to sort.
*/
void RecordArray_insertionSort(Record *records, int n) {
  InsertionSort is;

  InsertionSort_init(&is, &RecordArray_comparator, &RecordArray_swapper, sizeof(Record));
  InsertionSort_main(is, records, n);
}

/**
 * Selection sort.
 * 
 * @param   { Record * }  records   The records to sort.
 * @param   { int }       n         The number of records to sort.
*/
void RecordArray_selectionSort(Record *records, int n) {
  SelectionSort ss;

  SelectionSort_init(&ss, &RecordArray_comparator, &RecordArray_swapper, sizeof(Record));
  SelectionSort_main(ss, records, n);
}

/**
 * Merge sort.
 * 
 * @param   { Record * }  records   The records to sort.
 * @param   { int }       n         The number of records to sort.
*/
void RecordArray_mergeSort(Record *records, int n) {
  MergeSort ms;

  MergeSort_init(&ms, &RecordArray_comparator, &RecordArray_swapper, sizeof(Record));
  MergeSort_main(ms, records, n);
}

/**
 * Heap sort.
 * 
 * @param   { Record * }  records   The records to sort.
 * @param   { int }       n         The number of records to sort.
*/
void RecordArray_heapSort(Record *records, int n) {
  HeapSort hs;

  HeapSort_init(&hs, &RecordArray_comparator, &RecordArray_swapper, sizeof(Record));
  HeapSort_main(hs, records, n);
}

/**
 * Smooth sort.
 * 
 * @param   { Record * }  records   The records to sort.
 * @param   { int }       n         The number of records to sort.
*/
void RecordArray_smoothSort(Record *records, int n) {
  SmoothSort ss;

  SmoothSort_init(&ss, &RecordArray_comparator, &RecordArray_swapper, sizeof(Record));
  SmoothSort_main(ss, records, n);
}

/**
 * Momo sort.
 * 
 * @param   { Record * }  records   The records to sort.
 * @param   { int }       n         The number of records to sort.
*/
void RecordArray_momoSort(Record *records, int n) {
  MomoSort ms;

  MomoSort_init(&ms, &RecordArray_comparator, &RecordArray_swapper, sizeof(Record));
  MomoSort_main(ms, records, n);
}

#endif