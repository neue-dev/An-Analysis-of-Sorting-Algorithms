/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-10 19:44:13
 * @ Modified time: 2024-06-10 19:47:13
 * @ Description:
 * 
 * A small utility file for generating random numbers.
 */

#ifndef RANDOM_C
#define RANDOM_C

#include <stdlib.h>
#include <time.h>

/**
 * Generates a random number using the random function utility.
 * Also uses a different seed each time so the user doesn't need to specify this.
 * 
 * @param   { int }   n   The maximum number we want (inclusive).
 * @return  { int }       The random number we generated.
*/
int Random_generate(int n) {
  srand(time(NULL));

  return rand() % (n + 1);
}

#endif
