/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-15 12:46:43
 * @ Modified time: 2024-06-15 13:31:15
 * @ Description:
 * 
 * This file helps us log our data unto a file in a formatted manner.
 * We save our data as a bunch of CSV's for convenience.
 */

#include "./engine.c"
#include <stdio.h>

struct {
  char file[256];   // The base output filename
  FILE *pFile;

} LOGGER;

/**
 * Initializes the logger.
 * Specifies the output file to use and generates a handle to that file.
 * Note that if the file does not yet exist, it creates a new file.
 * 
 * @param   { char[] }  file  The base name of the out file.
*/
void Logger_init(char file[]) {
  strcpy(LOGGER.file, file);

  // Check if the file exists
  LOGGER.pFile = fopen(file, "r+");

  // If the file does not exist, create it
  if(LOGGER.pFile == NULL)
    fopen(file, "w+");
}

/**
 * Saves a single run onto the file.
 * 
 * @param   { Engine * }  engine  The engine whose data to save.
 * @param   { int }       run     The run we're saving.
*/
void Logger_saveRun(Engine *engine, int run) {
  int i;

  // We open the file
  LOGGER.pFile = fopen(LOGGER.file, "a+");
  
  // N, P, entropy, correlation
  fprintf(LOGGER.pFile, "%d, %lf, %lf, %lf",
    (int) engine->runs[0][run], // N
    engine->runs[1][run],       // P
    engine->runs[2][run],       // Entropy 
    engine->runs[3][run]);      // Correlation
  
  // The different sorter times
  // Note that these times are averages across cycles, hence %lf
  for(i = 0; i < SORTER_COUNT; i++)
    fprintf(LOGGER.pFile, ", %lf", engine->runs[_SORTER + i][run]);

  // New line
  fprintf(LOGGER.pFile, "\n");

  // Close the file
  fclose(LOGGER.pFile);
}

/**
 * Saves the runs as a new row unto the file.
 * 
 * @param   { Engine * }  engine  The engine whose data we'll save.
*/
void Logger_saveRuns(Engine *engine) {
  int i, j;

  // We save the runs
  for(i = 0; i < engine->runCount; i++)
    Logger_saveRun(engine, i);
}
