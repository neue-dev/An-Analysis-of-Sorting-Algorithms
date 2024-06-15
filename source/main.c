/**
 * @ Author: Mo David
 * @ Create Time: 2024-05-24 11:21:27
 * @ Modified time: 2024-06-15 16:16:23
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

// Import the logger
#include "./logger.c"

// Some useful libs
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int i, j;

  // Store the parameters given through the CLI
  CLI_initArgs(argc, argv);

  // Init the random number generator
  Random_init();

  // Init our sorters so we can use them
  Record_initSorters();

  // Init the Logger to save our data
  Logger_init(ARGS.out);

  // Create the tester engine and init it
  Engine engine;
  Engine_init(&engine);

  // Run the tester with the provided arguments
  // (or the default ones if none were given)
  for(i = 0; i < ARGS.NCount; i++) {
    for(j = 0; j < ARGS.PCount; j++) {
      
      // Reset the engine memory
      Engine_reset(&engine);

      // Run the engine for the given params
      Engine_doRuns(&engine, 
        ARGS.N[i], 
        ARGS.P[j], 
        ARGS.cycles, 
        ARGS.runs, 
        ARGS.algos);

      // Log the data
      Logger_saveRuns(&engine);
    }
  }

  return 0;
}