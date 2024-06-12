/**
 * @ Author: Mo David
 * @ Create Time: 2024-05-24 11:21:27
 * @ Modified time: 2024-06-12 11:36:26
 * @ Description:
 * 
 * The main file.
 */

// Import the needed utils
#include "./utils/sortingalgorithms.c"
#include "./utils/random.c"

// Import the tester
#include "./engine.c"

// Some useful libs
#include <stdio.h>
#include <stdlib.h>

int main() {

  // Init the random number generator
  Random_init();

  // Init our sorters so we can use them
  Record_initSorters();

  // Create the tester engine and init it
  Engine engine;
  Engine_init(&engine);

  // ! remove
  for(int i = 500000; i < 1000000; i *= 5) {
    for(double j = 1; j <= 1; j += 0.333) {
      Engine_run(&engine, i, j, 5, 2,
        SORTER_HEAP |
        SORTER_MERGE |
        SORTER_SMOOTH);
    }
  }

  return 0;
}