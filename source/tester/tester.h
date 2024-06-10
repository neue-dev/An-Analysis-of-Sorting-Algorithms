/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-10 12:35:40
 * @ Modified time: 2024-06-10 12:52:33
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

// A generic record (doesn't have to be the one defined by record.c)
typedef void * t_Record;

#endif