/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-10 19:44:13
 * @ Modified time: 2024-06-10 19:53:55
 * @ Description:
 * 
 * A small utility file for generating random numbers.
 */

#ifndef RANDOM_C
#define RANDOM_C

#include <stdlib.h>
#include <time.h>

/**
 * Initializes the seed to a different value each time we run the program.
*/
void Random_init() {
  srand(time(NULL));
}

/**
 * Generates a random number using the random function utility.
 * 
 * @param   { int }   n   The maximum number we want (inclusive).
 * @return  { int }       The random number we generated.
*/
int Random_generate(int n) {
  return rand() % (n + 1);
}

#endif
