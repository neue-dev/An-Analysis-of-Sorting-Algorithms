/**
 * @ Author: Mo David
 * @ Create Time: 2024-05-24 11:21:27
 * @ Modified time: 2024-06-11 21:36:06
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
  Engine_run(&engine, 100000, 0.5, 5, 10,
    SORTER_HEAP |
    // SORTER_MERGE |
    SORTER_SMOOTH);

  return 0;
}