/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-10 18:07:09
 * @ Modified time: 2024-06-15 14:12:24
 * @ Description:
 * 
 * Gives us utility functions for interacting with records.
 */

#ifndef RECORD_INTERFACE_C
#define RECORD_INTERFACE_C

#include "./filereader.c"
#include "./record.c"
#include "./random.c"
#include <stdlib.h>

#define RECORD_SIZE sizeof(Record)
#define DEFAULT_NAME "moo"

/**
 * This function compares two records.
 * We can swap this out for another function if we wish to redefine how we compare two records.
 * 
 * @param   { Record * }  records1  The first array of records.
 * @param   { Record * }  records2  The second array of records.
 * @param   { int }       i         The entry of the first array to check.
 * @param   { int }       j         The entry of the second array to check.
 * @return  { int }                 -1 means the r1 < r2; 1 means r1 > r2; 0 means r1 = r2.
*/
int Record_comparator(Record *records1, Record *records2, int i, int j) {
  if(records1[i].idNumber < records2[j].idNumber)
    return -1;

  if(records1[i].idNumber > records2[j].idNumber)
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
  Record r;

  // Swap the two
  r = records[i];
  records[i] = records[j];
  records[j] = r;
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
 * Note that record ids are generated randomly.
 * 
 * @param   { void * }  dest  The destination to receive the records.
 * @param   { int }     n     The number of records to generate.
 * @param   { int }     max   The maximum number of bytes to encode.
*/
void Record_fill(void *dest, int n, int max) {
  int size = 0;
  int i = 0;

  // Create a bunch of records and populate the array
  while(size + RECORD_SIZE < max && i < n) {
    Record r;

    // Create the record info
    r.idNumber = Random_generate(n);
    strcpy(r.name, DEFAULT_NAME);

    // Copy the created record to 
    memcpy(dest + size, &r, RECORD_SIZE);

    // Increment counter and size
    size += RECORD_SIZE;
    i++;
  }
}

/**
 * Reads the records from a file then transfers them unto the provided array.
 * 
 * @param   { void * }  dest  The destination of the reading process.
 * @param   { int }     n     The number of records to read.
 * @param   { int }     max   The maximum number of bytes to encode.
 * @param   { char[] }  file  The path to the file containing the records.     
*/
void Record_read(void *dest, int n, int max, char file[]) {
  int size = 0;
  int i = 0;

  // Temp memory for the records to create
  Record *records = calloc(n, RECORD_SIZE);

  // Read the file
  readFile(records, file);

  // Populate the array
  while(size + RECORD_SIZE < max && i < n) {
    Record r = records[i];

    // Copy the created record to 
    memcpy(dest + size, &r, RECORD_SIZE);

    // Increment counter and size
    size += RECORD_SIZE;
    i++;
  }

  // Free the temp memory
  free(records);
}

#endif

