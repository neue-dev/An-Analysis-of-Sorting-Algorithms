/**
 * @ Author: Mo David
 * @ Create Time: 2024-05-24 11:21:27
 * @ Modified time: 2024-06-15 01:17:56
 * @ Description:
 * 
 * The main file.
 */

// Import the needed utils
#include "./utils/sortingalgorithms.c"
#include "./utils/random.c"

// Import the cli file
#include "./cli.c"

// Import the tester
#include "./engine.c"

// Some useful libs
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  initArgs(argc, argv);
  return 0;

  // Init the random number generator
  Random_init();

  // Init our sorters so we can use them
  Record_initSorters();

  // Create the tester engine and init it
  Engine engine;
  Engine_init(&engine);

  // ! remove
  for(int i = 1024; i < 1000000; i *= 16) {
    for(double j = 0.333; j <= 1; j += 0.333) {
      Engine_run(&engine, i, j, 2, 2,
        SORTER_HEAP |
        SORTER_MERGE |
        SORTER_SMOOTH |
        SORTER_TIM
        );
    }
  }

  return 0;
}