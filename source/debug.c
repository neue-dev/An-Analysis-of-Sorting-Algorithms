#include "./utils/filereader.c"
#include "./utils/sortingalgorithms.c"
#include "./utils/record_interface.c"

#include <stdio.h>

/**
 * Copies the shuffle unto the array.
 * 
 * @param   { Record * }  records   The destination array.
 * @param   { Record * }  shuffle   The shuffle configuration to copy.
 * @param   { int }       N         The number of records to shuffle.
*/
void _Debug_copyShuffle(Record *records, Record *shuffle, int N) {
  int i;

  // Copy the entire array
  for(i = 0; i < N; i++)
    Record_copier(records, shuffle, i, i);
}

/**
 * Checks whether or not the sort is correct.
 * 
 * @param   { Record * }  records   The array to check.
 * @param   { Record * }  shuffle   The original shuffle configuration.
 * @param   { int }       N         The number of records to shuffle.
 * @return  { boolean }             Whether or not the sort was correct.
*/
int _Debug_checkSort(Record *records, Record *shuffle, int N) {
  int i, j;
  int comparison = 0;
  int order = 0;

  // Iterate through every pair of adjacent items
  for(i = 0; i < N - 1; i++) {

    // Get the result of the comparison
    comparison = Record_comparator(records, records, i, i + 1);

    // If the order has not been determined, set it
    if(!order)
      order = comparison;

    // It's okay for two adjacent elements to be equal
    // We just keep checking other elements if this is true
    if(!comparison)
      continue;

    // If they are not equal, however, they must be the same as the determined order
    // Otherwise, we have an instance of a flip in the list--it reverses sort order midway
    if(order != comparison)
      return 0;
  }

  // Check for missing entries
  for(i = 0; i < N; i++) {
    for(j = 0; j < N; j++) {

      // The entry isnt missing
      if(shuffle[i].idNumber == records[j].idNumber && 
        !strcmp(shuffle[i].name, records[j].name))
        break;
    }

    // There was a missing entry
    if(j == N)
      return 0;
  }

  // All checks were passed, so list is sorted
  return 1;
}

/**
 * Tests all the algorithms 5 times for the given shuffle and the given N.
 * 
 * @param   { Record * }  records   The array to shuffle.
 * @param   { Record * }  shuffle   The shuffle configuration to use.
 * @param   { int }       N         The number of records to shuffle.
*/
void _Debug_testAll(Record *records, Record *shuffle, int N) {
  int i, j, sorter;
  long start, end;
  double time;

  // Test label
  printf("# === TEST FOR N = %8d === #\n", N);

  // Perform the cycles
  for(i = 0; i < SORTER_COUNT; i++) {
    
    // The time we use to compute the average
    time = 0;

    for(j = 0; j < 10; j++) {

      // Get the sorter
      sorter = 1 << (_SORTER + i);

      // Copy the shuffle first
      _Debug_copyShuffle(records, shuffle, N);

      // Print update
      printf("[!] Running cycle %d for %s... ", j + 1, _Engine_getName(NULL, sorter));

      // Start the timer
      start = currentTimeMillis();

      // Execute the right algorithm
      switch(sorter) {

        case SORTER_HEAP:
          Record_heapSort(records, N);
          break;
        
        case SORTER_INSERTION:
          Record_insertionSort(records, N);
          break;
        
        case SORTER_MERGE:
          Record_mergeSort(records, N);
          break;
        
        case SORTER_SELECTION:
          Record_selectionSort(records, N);
          break;
        
        case SORTER_SMOOTH:
          Record_smoothSort(records, N);
          break;

        case SORTER_TIM:
          Record_timSort(records, N);
          break;

        default: break;
      }

      // Get the end time
      end = currentTimeMillis();

      // Get the total
      time += end - start;

      // Check for sort integrity
      if(_Debug_checkSort(records, shuffle, N))
        printf("sorted in %ld ms.\n", end - start);
      else printf("FAILED!\n");
    }

    printf("[$] %s took an average of %.4lf ms to sort N=%d\n\n", 
      _Engine_getName(NULL, sorter), time / 10, N);
  }

  printf("\n");
}

/**
 * The main debug function.
 * Produces the output in a text file.
*/
void Debug_main() { 
  int N;
  char path[500];
  Record *records = calloc(100000, sizeof(Record));
  Record *shuffle = calloc(100000, sizeof(Record));

  // Init the sorters first
  Record_initSorters();

  // Do it for the set of 100 first
  N = 100;
  strcpy(path, "./data/random100.txt");
  readFile(records, path);
  readFile(shuffle, path);

  // Test for each of the algorithms
  _Debug_testAll(records, shuffle, N);

  // Do it for the set of 25000
  N = 25000;
  strcpy(path, "./data/random25000.txt");
  readFile(records, path);
  readFile(shuffle, path);

  // Test for each of the algorithms
  _Debug_testAll(records, shuffle, N);

  // Do it for the set of 50000
  N = 50000;
  strcpy(path, "./data/random50000.txt");
  readFile(records, path);
  readFile(shuffle, path);

  // Test for each of the algorithms
  _Debug_testAll(records, shuffle, N);

  // Do it for the set of 75000
  N = 75000;
  strcpy(path, "./data/random75000.txt");
  readFile(records, path);
  readFile(shuffle, path);

  // Test for each of the algorithms
  _Debug_testAll(records, shuffle, N);

  // Do it for the set of 100000
  N = 100000;
  strcpy(path, "./data/random100000.txt");
  readFile(records, path);
  readFile(shuffle, path);

  // Test for each of the algorithms
  _Debug_testAll(records, shuffle, N);

  // Do it for the set of almostsorted.txt
  N = 100000;
  strcpy(path, "./data/almostsorted.txt");
  readFile(records, path);
  readFile(shuffle, path);

  // Test for each of the algorithms
  printf("ALMOST SORTED\n");
  _Debug_testAll(records, shuffle, N);

  // Do it for the set of totallyreversed.txt
  N = 100000;
  strcpy(path, "./data/totallyreversed.txt");
  readFile(records, path);
  readFile(shuffle, path);

  // Test for each of the algorithms
  printf("TOTALLY REVERSED\n");
  _Debug_testAll(records, shuffle, N);

  free(records);
  free(shuffle);
}