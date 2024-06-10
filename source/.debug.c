#include "./record.c"
#include <printf.h>
#include <math.h>

#ifndef DEBUG_C
#define DEBUG_C

/**
 * This method prints a single row of the heap.
 * We call this for each row in the main routine below.
 * 
 * @param   { Record  * }   records   The array of records.
 * @param   { int }         n         The number of records in the array.
 * @param   { int }         r         The row we want to print.
*/
void _Debug_printHeapRow(Record *records, int n, int r) {
  int i;

  // The start and end of the row
  int start = (1 << r) - 1;
  int end = start * 2;

  // Print the entries per row and a newline after
  for(i = start; i <= end && i < n; i++)
    printf("%8d ", records[i].idNumber);
  printf("\n");
}

/**
 * Print the array as a heap in a human-readable format.
 * 
 * @param   { Record * }  records   The array of records to print.
 * @param   { int }       n         The number of records in the array.
*/
void Debug_printHeap(Record *records, int n) {
  int i;

  // Count the total number of rows in the heap
  // Note that the C log() function computes the log with respect to base e
  int rows = (int) (log(n * 1.0) / log(2.0));

  // Account for the flooring error of the typecast above
  rows += 1 << rows <= n ? 1 : 0;

  // Call the function for each row
  for(i = 0; i < rows; i++)
    _Debug_printHeapRow(records, n, i);
}

/**
 * This method just prints the array as a column of numbers.
 * 
 * @param   { Record * }  records   The array to print.
 * @param   { int }       n         The number of records.
*/
void Debug_printArr(Record *records, int n) {
  int i;

  // Print each entry
  for(i = 0; i < n; i++)
    printf("(%8d) %8d %s\n", i, records[i].idNumber, records[i].name);
}

#endif