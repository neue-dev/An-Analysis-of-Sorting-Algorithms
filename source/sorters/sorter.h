/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-09 01:34:31
 * @ Modified time: 2024-06-10 12:38:44
 * @ Description:
 * 
 * Contains data types and templates needed by a sorting algorithm.
 * For instance, the data type for a comparator.
 */

#ifndef _SORT_CONFIG_C
#define _SORT_CONFIG_C

// A comparison function
// Signature should be: (t_Record records, int i, int j)
typedef int (* t_Comparator)();

// A swapper function
// Signature should be: (t_Record records, int i, int j)
typedef void (* t_Swapper)();

// A generic record (doesn't have to be the one defined by record.c)
typedef void * t_Record;

#endif