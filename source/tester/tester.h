/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-10 12:35:40
 * @ Modified time: 2024-06-11 00:18:35
 * @ Description:
 * 
 * Defines some types our tester need.
 * The t_Record and t_Comparator is a duplicate for sorter.h, but that's okay.
 * Tester and Sorter need not exist at the same time (as software pieces).
 * Thus, the permission for duplication.
 */

#ifndef _TEST_CONFIG_C
#define _TEST_CONFIG_C

// A comparison function
// Signature should be: (t_Record records, int i, int j)
typedef int (* t_Comparator)();

// A swapper function
// Signature should be: (t_Record records, int i, int j) -> void
typedef void (* t_Swapper)();

// A copier function
// Signature should be: (t_Record dest, t_Record src, int i, int j) -> void
typedef void (* t_Copier)();

// Returns the size of a single record
// Signature should be: (void) -> int
typedef int (* t_Sizer)();

// Populates a provided array with data points or records
// Signature should be: (t_Record dest, int n, int max) -> void
typedef void (* t_Filler)();

// Populates a provided array with records read from a file
// Signature should be: (t_Record dest, int n, int max, char file[]) -> void
typedef void (* t_Reader)();

// A generic record (doesn't have to be the one defined by record.c)
typedef void * t_Record;

#endif