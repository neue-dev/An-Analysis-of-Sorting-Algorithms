/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-10 18:07:09
 * @ Modified time: 2024-06-10 19:28:20
 * @ Description:
 * 
 * Gives us utility functions for interacting with records.
 */

#ifndef RECORD_INTERFACE_C
#define RECORD_INTERFACE_C

#include "./record.c"
#include <stdlib.h>
#include <time.h>

#define RECORD_SIZE sizeof(Record)

/**
 * This function compares two records.
 * We can swap this out for another function if we wish to redefine how we compare two records.
 * 
 * @param   { Record }  r1  The first record.
 * @param   { Record }  r2  The second record.
 * @return  { int }         -1 means the r1 < r2; 1 means r1 > r2; 0 means r1 = r2.
*/
int Record_comparator(Record *records, int i, int j) {
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
void Record_swapper(Record *records, int i, int j) {
  
  // Temp record
  Record rt;

  // Swap the two
  rt = records[i];
  records[i] = records[j];
  records[j] = rt;
}

/**
 * Returns the size of a single record in bytes
 * 
 * @return  { int }   The size of a single record in bytes.
*/
int Record_sizer() {
  return RECORD_SIZE;
}

/**
 * Copies the jth entry in src to the ith slot in dest.
 * 
 * @param   { t_Record }    dest      The destination array.
 * @param   { t_Record }    src       The source array.
 * @param   { int }         i         The ith slot of records.
 * @param   { int }         j         The jth entry in src.
*/
void Record_copier(Record *dest, Record *src, int i, int j) {

  // Copies the src record to the dest record.
  memcpy(
    dest + i, 
    src + j,
    RECORD_SIZE);
}

/**
 * Fills a provided array with random records.
 * Note that record ids are generated using the rand() function of the C standard library.
 * 
 * @param   { void * }  dest  The destination to receive the records.
 * @param   { int }     n     The number of records to generate.
 * @param   { int }     max   The maximum number of bytes to encode.
*/
void Record_fill(void *dest, int n, int max) {
  int size = 0;
  int i = 0;

  // Set the seed
  srand(time(NULL));

  // Create a bunch of records and populate the array
  while(size + RECORD_SIZE < max && i < n) {
    Record r;

    // Create the record info
    r.idNumber = rand() % n;
    strcpy(r.name, "mo");

    // Copy the created record to 
    memcpy(dest + size, &r, RECORD_SIZE);

    // Increment counter and size
    size += RECORD_SIZE;
    i++;
  }
}

#endif

