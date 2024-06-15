#include "./utils/filereader.c"
#include "./utils/sortingalgorithms.c"

#include <stdio.h>

int main() {
  int N = 10;
  char file[500] = "./data/random25000.txt";
  Record *records = calloc(100000, sizeof(Record));
  Record *basis = calloc(100000, sizeof(Record));

  // Call this first
  Record_initSorters();

  // Read file
  readFile(records, file);
  readFile(basis, file);

  // Print stuff
  printf("before: \n");
  for(int i = 0; i < N; i++)
    printf("%8d %s\n", records[i].idNumber, records[i].name);

  Record_smoothSort(records, N);

  printf("after: \n");
  for(int i = 0; i < N; i++)
    printf("%8d %s\n", records[i].idNumber, records[i].name);

  for(int i = 0; i < N; i++) {
    int j;
    for(j = 0; j < N; j++)
      if(basis[i].idNumber == records[j].idNumber)
        break;

    if(j == N)
      printf("Sorting failed... missing: %d %s\n", basis[i].idNumber, basis[i].name);
  }
  
  printf("End of sort details.\n");

  free(records);


  return 0;
}