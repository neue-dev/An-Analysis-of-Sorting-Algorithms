#include "record.c"
#include "sortingalgorithms.c"
#include "filereader.c"
#include "timer.c"
#include "./tester/tester.c"

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

int main() {

  // Create a new tester
  Tester tester;

  // Initialize the tester
  Tester_init(&tester, &Record_comparator, &Record_swapper, &Record_copier, &Record_sizer);

  // We will be using N = 100
  Tester_setN(&tester, 1000000);

  // Fill the tester with data
  Tester_recordsFill(&tester);

  // Perform some cleanup after
  Tester_exit(&tester);

  return 0;
}