/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-15 01:04:55
 * @ Modified time: 2024-06-15 12:50:24
 * @ Description:
 * 
 * This file deals with parsing the command line arguments fed into the program.
 */

#ifndef CLI_C
#define CLI_C

#include "./engine.c"

#include <string.h>
#include <ctype.h>

// This holds all our tester parameters
struct {

  // The tester parameters
  int algos;      // The algorithms to test
  int N[256];     // The number of records to sort
  double P[256];  // The shuffling factor
  int cycles;     // The number of cycles per run
  int runs;       // The number of runs

  // How many N and P values we have
  int NCount;
  int PCount;
} ARGS;

/**
 * Register the values of the algorithms to use.
 * 
 * @param   { char[] }  paramValue  The value of the CLI input arg after '='.
*/
void _CLI_registerAlgos(char paramValue[]) {
  char *iter = paramValue;
  char parameter[256];

  while(*iter) {
    parameter[strlen(parameter)] = *iter;

    // At the end of the string
    if(*(iter + 1) == ',' || *(iter + 1) == ' ' || !*(iter + 1)) {

      // Add the algo to the ones to test
      if(!strcmp(parameter, "heap") || !strcmp(parameter, "heapsort"))
        ARGS.algos |= SORTER_HEAP;

      if(!strcmp(parameter, "merge") || !strcmp(parameter, "mergesort"))
        ARGS.algos |= SORTER_MERGE;

      if(!strcmp(parameter, "insertion") || !strcmp(parameter, "insertionsort"))
        ARGS.algos |= SORTER_INSERTION;

      if(!strcmp(parameter, "selection") || !strcmp(parameter, "selectionsort"))
        ARGS.algos |= SORTER_SELECTION;

      if(!strcmp(parameter, "smooth") || !strcmp(parameter, "smoothsort"))
        ARGS.algos |= SORTER_SMOOTH;

      if(!strcmp(parameter, "tim") || !strcmp(parameter, "timsort"))
        ARGS.algos |= SORTER_TIM;

      // Clear the parameter
      while(strlen(parameter))
        parameter[strlen(parameter) - 1] = 0;

      // We aint ever reaching 256 chars right?
      iter++;
    }
    
    iter++;
  }
}

/**
 * Register the values of N to the ARGS struct.
 * 
 * @param   { char[] }  paramValue  The value of the CLI input arg after '='.
*/
void _CLI_registerN(char paramValue[]) {
  char *iter = paramValue;
  char parameter[256];

  while(*iter) {
    parameter[strlen(parameter)] = *iter;

    // At the end of the string
    if(*(iter + 1) == ',' || *(iter + 1) == ' ' || !*(iter + 1)) {

      // Save the value of N
      ARGS.N[ARGS.NCount++] = atoi(parameter);

      // Clear the parameter
      while(strlen(parameter))
        parameter[strlen(parameter) - 1] = 0;

      // We aint ever reaching 256 chars right?
      iter++;
    }
    
    iter++;
  }
}

/**
 * Register the value of P into the struct.
 * 
 * @param   { char[] }  paramValue  The value of the CLI input arg after '='.
*/
void _CLI_registerP(char paramValue[]) {
  char *iter = paramValue;
  char parameter[256];

  while(*iter) {
    parameter[strlen(parameter)] = *iter;

    // At the end of the string
    if(*(iter + 1) == ',' || *(iter + 1) == ' ' || !*(iter + 1)) {

      // Save the value of N
      ARGS.P[ARGS.PCount++] = atof(parameter);

      // Clear the parameter
      while(strlen(parameter))
        parameter[strlen(parameter) - 1] = 0;

      // We aint ever reaching 256 chars right?
      iter++;    
    }
    
    iter++;
  }
}

/**
 * Register the number of cycles into the struct.
 * 
 * @param   { char[] }  paramValue  The value of the CLI input arg after '='.
*/
void _CLI_registerCycles(char paramValue[]) {
  ARGS.cycles = atoi(paramValue);
}

/**
 * Register the number of runs into the struct.
 * 
 * @param   { char[] }  paramValue  The value of the CLI input arg after '='.
*/
void _CLI_registerRuns(char paramValue[]) {
  ARGS.runs = atoi(paramValue);
}

/**
 * Registers the parameter into the struct.
 * Parses the value based on the name of the parameter.
 * 
 * @param   { char[] }  paramName   The name of the parameter.
 * @param   { char[] }  paramValue  The value of the parameter.
*/
void _CLI_registerArg(char paramName[], char paramValue[]) {
  char *iter;
  char parameter[256];

  // We're saving the algos to use
  if(!strcmp(paramName, "algos") || 
    !strcmp(paramName, "algo") ||
    !strcmp(paramName, "algs") ||
    !strcmp(paramName, "alg") || 
    !strcmp(paramName, "a"))
      _CLI_registerAlgos(paramValue);

  // We're saving the N values
  if(!strcmp(paramName, "n"))
    _CLI_registerN(paramValue);

  // We're saving the P values
  if(!strcmp(paramName, "p"))
    _CLI_registerP(paramValue);

  // We're saving the number of cycles
  if(!strcmp(paramName, "cycles") || 
    !strcmp(paramName, "c"))
    _CLI_registerCycles(paramValue);

  // We're saving the number of runs
  if(!strcmp(paramName, "runs") || 
    !strcmp(paramName, "r"))
    _CLI_registerRuns(paramValue);
}

/**
 * Gets the command line arguments and defines the params from them.
 * 
 * @param   { int }       argc  The number of strings passed.
 * @param   { char *[] }  argv  The strings passed to the program.
*/
void CLI_initArgs(int argc, char *argv[]) {
  int i;
  char *iter;

  char buffer[1024] = { 0 };
  char word[1024] = { 0 };
  char parameter[1024] = { 0 };
  int isReadingWord = 1;

  // Init the counts
  ARGS.NCount = 0;
  ARGS.PCount = 0;
  ARGS.cycles = 1;
  ARGS.runs = 1;
  ARGS.algos = 0;

  // Go through each of the CLI args
  for(i = 1; i < argc; i++) {

    // Copy the string so we can do stuff with it
    strcpy(buffer, argv[i]);

    // We're reading the name of the parameter
    isReadingWord = 1;
    iter = buffer;

    // Parse the word
    while(*iter) {

      // Get the equal sign
      if(*iter == '=') {
        isReadingWord = 0;
        iter++;
        
        continue;
      }

      // Update the word
      if(isReadingWord)
        word[strlen(word)] = tolower(*iter);

      // Update the parameter
      else
        parameter[strlen(parameter)] = tolower(*iter);

      // It's the end of the word
      if(*(iter + 1) == ' ' || !*(iter + 1)) {

        // Register the parameter
        _CLI_registerArg(word, parameter);

        // Clear the name and the parameter
        while(strlen(word))
          word[strlen(word) - 1] = '\0';
        
        while(strlen(parameter))
          parameter[strlen(parameter) - 1] = '\0';

        // We aint ever reaching 1024 chars right?
        iter++;
      }

      iter++;
    }
  }

  // Set default algos to use
  if(!ARGS.algos)
    ARGS.algos = 
      SORTER_HEAP |
      SORTER_MERGE |
      SORTER_INSERTION | 
      SORTER_SELECTION | 
      SORTER_SMOOTH | 
      SORTER_TIM;

  // Set default value of N
  if(!ARGS.NCount)
    ARGS.N[ARGS.NCount++] = 1;

  // Set default value of P
  if(!ARGS.PCount)
    ARGS.P[ARGS.PCount++] = 1;
}

#endif