/**
 * @ Author: Mo David
 * @ Create Time: 2024-05-24 11:21:27
 * @ Modified time: 2024-06-20 15:42:05
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
#include "../sorters/tim_sort.c"

// So we don't have to deal with bugs
int isInitted = 0;

// Our different sorters
InsertionSort _Record_IS;
SelectionSort _Record_LS;
MergeSort _Record_MS;
HeapSort _Record_HS;
SmoothSort _Record_SS;
TimSort _Record_TS;

/**
 * Initializes our sorters.
*/
void Record_initSorters() {
  
  // For warming up, idkk
  // Now this is fucking insane but for some reason smooth sort is x2 slower without this initial run??
  // It has something to do with my implementation, but trust me I've analyzed for hours and can't figure out what it is
  Record r; r.idNumber = 0;
  Record record[1] = { r };

  // Initialize the different sorter objects
  InsertionSort_init(
    &_Record_IS, &Record_comparator, &Record_swapper, &Record_copier, &Record_sizer);
  SelectionSort_init(
    &_Record_LS, &Record_comparator, &Record_swapper, &Record_copier, &Record_sizer);
  MergeSort_init(
    &_Record_MS, &Record_comparator, &Record_swapper, &Record_copier, &Record_sizer);
  HeapSort_init(
    &_Record_HS, &Record_comparator, &Record_swapper, &Record_copier, &Record_sizer);
  SmoothSort_init(
    &_Record_SS, &Record_comparator, &Record_swapper, &Record_copier, &Record_sizer);
  TimSort_init(
    &_Record_TS, &Record_comparator, &Record_swapper, &Record_copier, &Record_sizer);

  // Warp up smooth sort
  SmoothSort_main(&_Record_SS, record, 1);

  // It's done
  isInitted = 1;
}

/**
 * Warn the user that they haven't initted the sorters.
*/
void _Record_warn() {
  printf("\n\n!!! ERROR: Record_initSorters() was not called yet!\n");
  printf("!!! ERROR: Please call it before running any tests of your own.\n");
  printf("!!! ERROR: Refer to the README for more information on this.\n\n");
}


/**
 * Insertion sort.
 * 
 * @param   { Record * }  records   The records to sort.
 * @param   { int }       n         The number of records to sort.
*/
void Record_insertionSort(Record *records, int n) {
  if(!isInitted) _Record_warn();
  else InsertionSort_main(&_Record_IS, records, n);
}

/**
 * Selection sort.
 * 
 * @param   { Record * }  records   The records to sort.
 * @param   { int }       n         The number of records to sort.
*/
void Record_selectionSort(Record *records, int n) {
  if(!isInitted) _Record_warn();
  else SelectionSort_main(&_Record_LS, records, n);
}

/**
 * Merge sort.
 * 
 * @param   { Record * }  records   The records to sort.
 * @param   { int }       n         The number of records to sort.
*/
void Record_mergeSort(Record *records, int n) {
  if(!isInitted) _Record_warn();
  else MergeSort_main(&_Record_MS, records, n);
}

/**
 * Heap sort.
 * 
 * @param   { Record * }  records   The records to sort.
 * @param   { int }       n         The number of records to sort.
*/
void Record_heapSort(Record *records, int n) {
  if(!isInitted) _Record_warn();
  else HeapSort_main(&_Record_HS, records, n);
}

/**
 * Smooth sort.
 * 
 * @param   { Record * }  records   The records to sort.
 * @param   { int }       n         The number of records to sort.
*/
void Record_smoothSort(Record *records, int n) {
  if(!isInitted) _Record_warn();
  else SmoothSort_main(&_Record_SS, records, n);
}

/**
 * Tim sort.
 * 
 * @param   { Record * }  records   The records to sort.
 * @param   { int }       n         The number of records to sort.
*/
void Record_timSort(Record *records, int n) {
  if(!isInitted) _Record_warn();
  else TimSort_main(&_Record_TS, records, n);
}

/**
 * Insertion sort frequency count.
 * 
 * @return  { long }  The frequency count incurred by insertion sort.
*/
long Record_getInsertionSortFreq() {
  if(!isInitted) _Record_warn();
  else return _Record_IS.frequencyCount;
  return -1;
}

/**
 * Selection sort frequency count.
 * 
 * @return  { long }  The frequency count incurred by selection sort.
*/
long Record_getSelectionSortFreq() {
  if(!isInitted) _Record_warn();
  else return _Record_LS.frequencyCount;
  return -1;
}

/**
 * Merge sort frequency count.
 * 
 * @return  { long }  The frequency count incurred by merge sort.
*/
long Record_getMergeSortFreq() {
  if(!isInitted) _Record_warn();
  else return _Record_MS.frequencyCount;
  return -1;
}

/**
 * Heap sort frequency count.
 * 
 * @return  { long }  The frequency count incurred by heap sort.
*/
long Record_getHeapSortFreq() {
  if(!isInitted) _Record_warn();
  else return _Record_HS.frequencyCount;
  return -1;
}

/**
 * Smooth sort frequency count.
 * 
 * @return  { long }  The frequency count incurred by smooth sort.
*/
long Record_getSmoothSortFreq() {
  if(!isInitted) _Record_warn();
  else return _Record_SS.frequencyCount;
  return -1;
}

/**
 * Tim sort frequency count.
 * 
 * @return  { long }  The frequency count incurred by tim sort.
*/
long Record_getTimSortFreq() {
  if(!isInitted) _Record_warn();
  else return _Record_TS.frequencyCount;
  return -1;
}

#endif