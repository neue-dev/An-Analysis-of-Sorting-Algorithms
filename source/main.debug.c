#include "./utils/filereader.c"
#include "./utils/sortingalgorithms.c"

#include <stdio.h>

int main() {
  int N = 15;
  char file[500] = "./data/random100.txt";
  Record *records = calloc(10000, sizeof(Record));
  
  // Call this first
  Record_initSorters();

  // Read file
  readFile(records, file);

  // Print stuff
  printf("before: \n");
  for(int i = 0; i < N; i++)
    printf("%8d %s\n", records[i].idNumber, records[i].name);

  Record_heapSort(records, N);

  printf("after: \n");
  for(int i = 0; i < N; i++)
    printf("%8d %s\n", records[i].idNumber, records[i].name);

  free(records);


  return 0;
}