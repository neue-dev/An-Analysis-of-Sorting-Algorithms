/**
 * @ Author: Mo David
 * @ Create Time: 2024-05-24 11:21:27
 * @ Modified time: 2024-06-12 00:58:34
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
#include "./record.c"
#include "./record_interface.c"

// Import the different sorting algorithms
#include "../sorters/insertion_sort.c"
#include "../sorters/selection_sort.c"
#include "../sorters/merge_sort.c"
#include "../sorters/heap_sort.c"
#include "../sorters/smooth_sort.c"

// Our different sorters
InsertionSort is;
SelectionSort ls;
MergeSort ms;
HeapSort hs;
SmoothSort ss;

/**
 * Initializes our sorters.
*/
void Record_initSorters() {
  
  // For warming up, idkk
  // Now this is fucking insance but for some reason smooth sort is x2 slower without this initial run??
  // It has something to do with my implementation, but trust me I've analyzed for hours and can't firgure out what it is
  Record r; r.idNumber = 0;
  Record record[1] = { r };

  // Initialize the different sorter objects
  InsertionSort_init(&is, &Record_comparator, &Record_swapper, &Record_copier, &Record_sizer);
  SelectionSort_init(&ls, &Record_comparator, &Record_swapper, &Record_copier, &Record_sizer);
  MergeSort_init(&ms, &Record_comparator, &Record_swapper, &Record_copier, &Record_sizer);
  HeapSort_init(&hs, &Record_comparator, &Record_swapper, &Record_copier, &Record_sizer);
  SmoothSort_init(&ss, &Record_comparator, &Record_swapper, &Record_copier, &Record_sizer);

  // Warp up smooth sort
  SmoothSort_main(ss, record, 1);
}

/**
 * Insertion sort.
 * 
 * @param   { Record * }  records   The records to sort.
 * @param   { int }       n         The number of records to sort.
*/
void Record_insertionSort(Record *records, int n) {
  InsertionSort_main(is, records, n);
}

/**
 * Selection sort.
 * 
 * @param   { Record * }  records   The records to sort.
 * @param   { int }       n         The number of records to sort.
*/
void Record_selectionSort(Record *records, int n) {
  SelectionSort_main(ls, records, n);
}

/**
 * Merge sort.
 * 
 * @param   { Record * }  records   The records to sort.
 * @param   { int }       n         The number of records to sort.
*/
void Record_mergeSort(Record *records, int n) {
  MergeSort_main(ms, records, n);
}

/**
 * Heap sort.
 * 
 * @param   { Record * }  records   The records to sort.
 * @param   { int }       n         The number of records to sort.
*/
void Record_heapSort(Record *records, int n) {
  HeapSort_main(hs, records, n);
}

/**
 * Smooth sort.
 * 
 * @param   { Record * }  records   The records to sort.
 * @param   { int }       n         The number of records to sort.
*/
void Record_smoothSort(Record *records, int n) {
  SmoothSort_main(ss, records, n);
}

#endif