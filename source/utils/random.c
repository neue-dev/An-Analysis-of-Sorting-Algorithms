/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-10 19:44:13
 * @ Modified time: 2024-06-11 02:26:40
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

/**
 * Returns a boolean based on a provided probability.
 * The provided probability is the chance of returning a 1.
 * 
 * @param   { double }  p   The probability (between 0 and 1, inclusive) of returning a 1.
 * @return  { int }         A probabilistic 0 or 1.
*/
int Random_probability(double p) {
  int max = (1ULL << 30);

  // p cannot be greater than 1
  if(p > 1.0)
    p = 1.0;

  // p cannot be negative
  if(p < 0)
    p = 0.0;

  // Probabilistic 1 or 0
  if(Random_generate(max) <= max * p)
    return 1;
  return 0;
}

#endif
