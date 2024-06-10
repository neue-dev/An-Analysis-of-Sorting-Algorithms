/**
 * @ Author: Mo David
 * @ Create Time: 2024-05-24 11:21:27
 * @ Modified time: 2024-06-11 00:27:16
 * @ Description:
 * 
 * The main file.
 */

// Import the needed utils
#include "./utils/sortingalgorithms.c"
#include "./utils/random.c"

// Import the tester
#include "./tester/tester.c"

// Some useful libs
#include <stdio.h>
#include <stdlib.h>

int main() {

  // Init the random number generator
  Random_init();

  // Create a new tester
  Tester tester;

  // Initialize the tester
  Tester_init(&tester, &Record_comparator, &Record_swapper, &Record_copier, &Record_sizer);

  // We will be using N = 100
  Tester_setN(&tester, 1000);
  Tester_setP(&tester, 0.01);

  // Fill the tester with data then configure it
  Tester_recordsFill(&tester, &Record_fill);
  Tester_recordsShuffle(&tester);
  
  printf("Sorted? %s\n", Tester_checkSort(&tester) ? "yes" : "no");

  // Perform some cleanup after
  Tester_exit(&tester);

  return 0;
}