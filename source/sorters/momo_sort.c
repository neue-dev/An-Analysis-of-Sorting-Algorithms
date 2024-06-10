/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-10 02:49:16
 * @ Modified time: 2024-06-10 12:54:15
 * @ Description:
 * 
 * My own sorting algorithm, inspired by smooth sort.
 * This is an adaptive sorting algorithm that uses binary heaps instead of Leonardo heaps.
 * Also, it uses a constant amount O(1) of auxiliary space (16 extra bytes).
 */

#ifndef MOMO_SORT_C
#define MOMO_SORT_C

#include "./sorter.h"

typedef struct MomoSort {
  
  t_Comparator comparator;
  t_Swapper swapper;
  int recordSize;

  // 'Private' variables
  int _powers2Less1List[31];
  int _powers2Less1Count;

} MomoSort;

/**
 * Initializes the momo sort data object.
 * 
 * @param   { MomoSort * }    this        The momo sort data object.      
 * @param   { t_Comparator }  comparator  The comparator to use for sorting.
 * @param   { t_Swapper }     swapper     The swapper to use for sorting.
 * @param   { int }           recordSize  The size of each record.
*/
void MomoSort_init(MomoSort *this, t_Comparator comparator, t_Swapper swapper, int recordSize) {
  int i;

  this->comparator = comparator;
  this->swapper = swapper;
  this->recordSize = recordSize;

  // Set the number of powers of 2
  this->_powers2Less1Count = 31;

  // Compute the powers of two minus 1
  for(i = 0; i < this->_powers2Less1Count; i++)
    this->_powers2Less1List[i] = (1 << (i + 1)) - 1;
}

/**
 * Returns the kth 2^n - 1.
 * 
 * @param   { MomoSort }  this  The momo sort data object.
 * @param   { int }       k     The index of the power we want.
 * @return  { int }             The kth 2^n - 1
*/
int _MomoSort_powers2Less1Get(MomoSort this, int k) {
  
  // If beyond list bounds, return last num
  if(k >= this._powers2Less1Count)
    return this._powers2Less1List[this._powers2Less1Count - 1];

  // Return the kth power of 2 minus 1
  return this._powers2Less1List[k];
}

void _MomoSort_siftDown() {
  
}

void _MomoSort_insert(MomoSort *this, t_Record records, int n) {
  
}

/**
 * Sorts a given array of records using momo sort.
 * Momo sort works by converting the array into a forest of binary heaps.
 * 
 * Basically, since any number can be represented as a sum of unique powers of two (in other words,
 * every number has a binary representation OR is equal to 2 * (2^k) for some k), we can convert any 
 * n-length array into a unique forest of binary max-heaps. If we make sure the roots of these heaps 
 * are in ascending order, then we can perform the same form of sorting smooth sort does (except with 
 * binary heaps so things are hopefully less complex). 
 * 
 * @param   { Momosort }  this      The momo sort data object.
 * @param   { t_Record }  records   The array of records to sort.
 * @param   { int }       n         The number of items in the array.
*/
void MomoSort_main(MomoSort this, t_Record records, int n) {
  int i;

  // This help us keep track of the sequence of binary heaps 
  long bseq1 = 0, lfirst1;
  long bseq2 = 0, lfirst2;

  // We convert the array into a forest of binary max-heaps
  for(i = 0; i < n; i++) {

    // Compute the rightmost bits for each
    lfirst1 = bseq1 & -bseq1;
    lfirst2 = bseq2 & -bseq2;

    // Check if least significant bits are equal first (AND not 0)
    // Note that all conds are mutually exclusive since if lfirst1 != lfirst2 then 
    //    either bseq1 % 2 == 0 or bseq2 % 2 == 0
    if(lfirst1 == lfirst2 && lfirst1 && lfirst2) {
      
      // We make sure to occupy bseq1 first before bseq2
      if(bseq1 & (lfirst1 << 1)) {
        bseq2 |= lfirst2 << 1;
      
      // Otherwise, fill in bseq1
      } else {
        bseq1 |= lfirst1 << 1;
      }

      // Unset the rightmost bits
      bseq1 &= bseq1 -1;
      bseq2 &= bseq2 - 1;
    
    // Check first if bseq1 has a free bit
    // We must check bseq1 before checking bseq2 for a free bit
    } else if(bseq1 % 2 == 0) {
      bseq1++;
    
    // Check if bseq2 has a free bit
    } else if(bseq2 % 2 == 0) {
      bseq2++;
    }

    // ! todo: insert the current element into slice and heapify 
  }

  // Perform the sorting starting from the end
  for(i = n; --i >= 0;) {
    
    // ! todo: pop off the rightmost node and fix the new nodes creating by the popping 
  }
}

#endif