#include "record.c"
#include "sortingalgorithms.c"
#include "filereader.c"
#include "timer.c"
#include ".debug.c"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

int main() {

  int i;
  int N = 15;
  char path[500] = "./data/random100000.txt";
  Record *records = calloc(1000000, sizeof(Record));

  // Read the records from the file
  readFile(records, path);

  // // Display the initial order of records
  // printf("\n=== INITIAL RECORDS ===\n");
  // RecordArray_display(records, N);

  // // Perform insertion sort on the data
  // // RecordArray_insertionSort(records, N);
  // RecordArray_mergeSort(records, N);

  // // Display the records
  // printf("\n=== SORTED RECORDS ===\n");
  // RecordArray_display(records, N);



  srand(time(NULL));

  int dummy[7] = {0, 1, 1, 3, 1, 3, 6};
  for(int i = 0; i < 1; i++) {
    int start, end;
    for(int j = 0; j < N; j++) {
      Record r;
      r.idNumber = rand() % N;
      // r.idNumber =  (rand() % (j + 1)) * (j + 1);
      // r.idNumber = j % 2 ? (N - j) : j;
      // r.idNumber = ((int) powl(7, j)) % N; 
      // r.idNumber = dummy[j];
      sprintf(r.name, "%s%d", "hello", j);
      records[j] = r;
    }

    printf("Before: \n");
    Debug_printArr(records, N);

    // RecordArray_mergeSort(records, N);

    start = currentTimeMillis();
    // switch(i) {
    //   case 0:
    //     RecordArray_mergeSort(records, N);
    //     break;

    //   case 1:
    //     RecordArray_heapSort(records, N);
    //     break;

    //   case 2:
    //     RecordArray_smoothSort(records, N);
    //     break;
    // }
    RecordArray_smoothSort(records, N);
    end = currentTimeMillis();

    printf("After: \n");
    Debug_printArr(records, N);

    printf("time: %d ms\n", end - start);

  }

  // int limit = N;
  // Debug_printArr(records, limit);
  // RecordArray_smoothSort(records, limit);
  // printf("SORTED:\n");
  // Debug_printArr(records, limit);

  // Don't forget to free the memory!
  free(records);

  return 0;
}