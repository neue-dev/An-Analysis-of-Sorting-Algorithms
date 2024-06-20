/**
 * @ Author: Mo David
 * @ Create Time: 2024-05-24 11:21:27
 * @ Modified time: 2024-06-21 03:56:09
 * @ Description:
 * 
 * The main file.
 * Now I usually expound quite a bit in this file as per tradition but I will only leave my acknowledgements
 * here due to a lack of time and preparation. Nevertheless, don't let the brevity of this epigraph undermine
 * the stature of this project.
 * 
 * [ Fisher-Yates Shuffling ]                                https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
 * [ Heap Sort According to Wikipedia ]                      https://en.wikipedia.org/wiki/Heapsort
 * [ Measuring the Entropy of a Deck of Cards ]              https://stats.stackexchange.com/questions/78591/correlation-between-two-decks-of-cards/79552#79552
 * [ Merge Sort According to Wikipedia ]                     https://en.wikipedia.org/wiki/Merge_sort
 * [ Merge Sort with 0 Aux Space ]                           https://www.geeksforgeeks.org/in-place-merge-sort/
 * [ Proper Random Numbers in C ]                            https://stackoverflow.com/questions/822323/how-to-generate-a-random-int-in-c
 * [ Smooth Sort Demystified ]                               https://www.keithschwarz.com/smoothsort/#:~:text=Moreover%2C%20smoothsort%20is%20an%20in,space%20in%20the%20worst%20case.
 * [ Speedy Log2 Implementation ]                            https://stackoverflow.com/questions/11376288/fast-computing-of-log2-for-64-bit-integers
 * [ The Original Smooth Sort Paper by Dijkstra Himself! ]   https://citeseerx.ist.psu.edu/document?repid=rep1&type=pdf&doi=bec9f897d701a8326ccdfc97064f4ce2c07a732f
 * [ Quote of Claude Shannon on Entropy ]                    https://mathoverflow.net/questions/403036/john-von-neumanns-remark-on-entropy
 * 
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

// Import the debug file
#include "./debug.c"

// Some useful libs
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int i, j;

  // We're not running the debug routine
  if(argc == 1 || strcmp(argv[1], "debug")) {

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

    // Clean up the state and memory
    Engine_exit(&engine);

  // Run the debug routine
  } else {
    Debug_main();
  }

  return 0;
}