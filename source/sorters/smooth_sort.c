/**
 * @ Author: Mo David
 * @ Create Time: 2024-06-09 01:32:10
 * @ Modified time: 2024-06-15 15:47:05
 * @ Description:
 * 
 * An implementation of smoort sort.
 */

#ifndef SMOOTH_SORT_C
#define SMOOTH_SORT_C

#include "./sorter.h"

/**
 * Holds information about the smooth sort algorithm we'll be using.
 * The comparator tells smooth sort how to compare two entries.
 * The swapper tells smooth sort how to swap two entries.
*/
typedef struct SmoothSort {
  
  t_Comparator comparator;
  t_Swapper swapper;
  t_Copier copier;
  t_Sizer sizer;
  
  // 'Private' variables
  int _leonardoList[43];
  int _leonardoCount;
  
} SmoothSort;

/**
 * Initializes the smoort sort data object.
 * Precomputes the Leonardo numbers, which we use throughout the sort.
 * Leonardo numbers are defined by:
 * 
 *  L(0) = 1
 *  L(1) = 1
 *  L(k) = L(k - 1) + L(k - 2) + 1
 * 
 * @param   { SmoothSort * }  this        The smooth sort data object to init.
 * @param   { t_Comparator }  comparator  The comparator to use for sorting.
 * @param   { t_Swapper }     swapper     The swapper to use for sorting.
 * @param   { t_Copier }      copier      The copier to use in case needed.
 * @param   { t_Sizer }       sizer       A function that returns the size of a record.
*/
void SmoothSort_init(SmoothSort *this, t_Comparator comparator, t_Swapper swapper, t_Copier copier, t_Sizer sizer) {
  int i;

  // Comparator and swapper
  this->comparator = comparator;
  this->swapper = swapper;
  this->copier = copier;
  this->sizer = sizer;

  // Init the state of the first two Leonardo numbers
  this->_leonardoList[0] = 1;
  this->_leonardoList[1] = 1;

  // How many Leonardo numbers we want to generate
  this->_leonardoCount = 43;
  
  // Generate all the Leonardo numbers til the max allowed.
  for(i = 2; i < this->_leonardoCount; i++)
    this->_leonardoList[i] = 1 + 
      this->_leonardoList[i - 1] +
      this->_leonardoList[i - 2];
}

/**
 * Returns the kth Leonardo number.
 * This is a helper function, as indicated by the prefix '_'.
 * 
 * @param   { SmoothSort }  this  The smooth sort data object.
 * @param   { int }         k     The index of the Leonardo number we want.
 * @return  { int }               The kth Leonardo number.
*/
int _SmoothSort_leonardoGet(SmoothSort this, int k) {

  // Return the last Leonardo number if k is too big
  if(k >= this._leonardoCount)
    return this._leonardoList[this._leonardoCount - 1];
  
  // Return the value
  return this._leonardoList[k];
}

/**
 * Returns the position of the root node of the current Leonardo heap.
 * The current Leonardo heap is of order k.
 * 
 * @param   { SmoothSort }  this  The smooth sort data object.
 * @param   { int }         k     The order of the current Leonardo heap.
 * @return  { int }               The index of the root.
*/
int _SmoothSort_getRootOffset(SmoothSort this, int k) {
  return _SmoothSort_leonardoGet(this, k);
}

/**
 * Returns the position of the first child of the root node of the current Leonardo heap.
 * The current Leonardo heap is of order k.
 * Returns the position of the root when the heap only has a root node.
 * 
 * @param   { SmoothSort }  this  The smooth sort data object.
 * @param   { int }         k     The order of the current Leonardo tree.
 * @return  { int }               The index of the root's first child.
*/
int _SmoothSort_getChild1Offset(SmoothSort this, int k) {
  if(k < 2)
    return _SmoothSort_getRootOffset(this, k);
  return _SmoothSort_leonardoGet(this, k - 1);
}

/**
 * Returns the position of the second child of the root node of the current Leonardo heap.
 * The current Leonardo heap is of order k.
 * Returns the position of the root when the heap only has a root node.
 * 
 * @param   { SmoothSort }  this  The smooth sort data object.
 * @param   { int }         k     The order of the current Leonardo tree.
 * @return  { int }               The index of the root's second child.
*/
int _SmoothSort_getChild2Offset(SmoothSort this, int k) {
  if(k < 2)
    return _SmoothSort_getRootOffset(this, k);
  return _SmoothSort_leonardoGet(this, k) - 1;
}

/**
 * Sifts a new root down a Leonardo heap of order k (starting from index 'start' in the main array)
 * untils it reaches its resting point (that is, it fixed a given Leonardo heap when its root is
 * replaced; in this case, only the root has to be sifted down if the Leonardo max-heap was valid
 * to begin with).
 * 
 * @param   { SmoothSort }  this      The smooth sort data object.
 * @param   { t_Record }    records   The array containing the data we want to modify.
 * @param   { int }         n         The number of records we have.
 * @param   { t_Record }    rRoot     The current root record.
 * @param   { int }         k         The order of the current Leonardo heap.
 * @param   { int }         start     The index that marks the leftmost member of the heap in the array.
 * @param   { int }         root      The index of the root of the current heap (also the end of the 
 *                                      array slice we're considering)
*/
void _SmoothSort_siftDown(SmoothSort this, t_Record records, int n, t_Record rRoot, int k, int start, int root) {
  int c1, c2, largest;

  // There's no children to sift with anymore
  // Copy the temp unto its location
  if(k < 2) {
    this.copier(records, rRoot, root, 0);
    
    return;
  }

  // Stores which of the nodes (root, c1, or c2) is the largest
  largest = root;
  
  // Get the children indices
  c1 = _SmoothSort_getChild1Offset(this, k) + start;
  c2 = _SmoothSort_getChild2Offset(this, k) + start;
  
  // Get the larger of the two children
  if(this.comparator(records, records, c2, c1) > 0)
    largest = c2;
  else
    largest = c1;

  // If both children are less than the root
  if(this.comparator(records, rRoot, largest, 0) < 0)
    largest = root;

  // ! remove
  // // Compare with the two children and swap with the larger one
  // if(this.comparator(records, records, c1, largest) > 0)
  //   largest = c1;

  // if(this.comparator(records, records, c2, largest) > 0)
  //   largest = c2;
    
  // If no need to swap, return after performing the last sift
  if(largest == root) {
    this.copier(records, rRoot, largest, 0);

    return;
  }

  // Otherwise, do the swap
  this.copier(records, records, root, largest);

  // Then call the function again, based on which child was swapped
  if(c1 != c2) {

    // If swapped with first child, left subtree still starts at 'start'
    // But this time it has order k - 1 and root at c1
    if(largest == c1) 
      _SmoothSort_siftDown(this, records, n, rRoot, k - 1, start, c1);

    // If swapped with second child, the right subtree starts after the left subtree
    // Thus, we have the offset; also, the order is now k - 2
    if(largest == c2)
      _SmoothSort_siftDown(this, records, n, rRoot, k - 2, start + _SmoothSort_leonardoGet(this, k - 1), c2);
  }
}

/**
 * Inserts an element into the heapified portion of the array and fixes the heaps to maintain this 
 * property. At the start of insertion, note that the new element automatically becomes the root
 * of the smallest Leonardo heap in our sequence.
 * 
 * @param   { SmoothSort }    this      The smooth sort data object.
 * @param   { t_Record }      records   The array of records to use.
 * @param   { int }           n         The total number of records.
 * @param   { int }           i         We're inserting the ith element of the original array into
 *                                        the [0, i - 1] slice.
 * @param   { unsigned int }  lseq      Information about which Leonardo numbers we're using.
*/
void _SmoothSort_insert(SmoothSort this, t_Record records, int n, int i, unsigned int lseq) {
  int bit, exp, order, porder;    // Holding variables
  int c1, c2, root, new;          // Children and root variables, newly inserted element

  // l represents our current offset from the end of the heapified slice of the array
  // l will always point to a root of a Leonardo heap, based on our calculations below
  int l = 0;

  // k will be stored in order
  // The variable "order" represents the current order of the Leonardo heap l is pointing to
  int k = 0;

  // Temp variables, one for sifting the other for insertion sorting the roots
  t_Record r = calloc(1, this.sizer());
  
  // Init this first
  porder = -1;

  // If there's only one Leonardo heap, just sift it already
  // The condition just checks if lseq only has a single rightmost bit
  // The return here is just to exit the function prematurely; it doesn't return anything
  if(!(lseq - (lseq & -lseq))) {
    exp = (int) (log(lseq) / log(2));

    // Grab the root
    this.copier(r, records, 0, i);

    // Call the sifter
    _SmoothSort_siftDown(this, records, n, r, exp, -1, i);

    return;
  }

  // We go through the lengths 
  while(lseq) {

    // Get the current bit and the current order
    bit = lseq % 2;
    order = k;
    
    // Update lseq and k
    lseq >>= 1;
    k++;

    // We don't have a Leonardo heap of that order
    if(!bit)
      continue;

    // Otherwise, update l if we do
    l += _SmoothSort_leonardoGet(this, order);

    // If this is the first iteration, we skip it for now
    if(porder < 0) {
      porder = order;
      continue;
    }

    // Then, we swap the inserted element with the root on the heap to the left IF
    //  (1) this root is greater than the inserted element AND
    //  (2) this root is greater than the children of that inserted element
    root = _SmoothSort_getRootOffset(this, order) + i - l;
    c1 = _SmoothSort_getChild1Offset(this, porder) + root;
    c2 = _SmoothSort_getChild2Offset(this, porder) + root;
    new = _SmoothSort_getRootOffset(this, porder) + root;

    // Do the swap
    if(this.comparator(records, records, root, new) > 0 && 
      this.comparator(records, records, root, c1) > 0 &&
      this.comparator(records, records, root, c2) > 0) {

      // Swap the two roots
      this.swapper(records, root, new);

      // If this is the last iteration, better fix the heap structure now
      if(!lseq) {
        
        // Grab the root
        this.copier(r, records, 0, root);

        // Sift down the root
        _SmoothSort_siftDown(this, records, n, r, order, i - l, root);
      }

    // If no swap happened, then it means the roots are in ascending order already
    // We can proceed to heapifying the heap we last swapped with 
    // (we do this in another routine)
    } else {

      // Grab the root
      this.copier(r, records, 0, new);
      
      // Note that we're considering the slice from [root, new] since no swap occured
      _SmoothSort_siftDown(this, records, n, r, porder, root, new);
      return; 
    }

    // Finally, save the order into porder (previous order) for later retrieval
    porder = order;
  }

  // Free the temp variable
  free(r);
}

/**
 * For a given integer n, if lseq represents the members of the Leonardo sequence that sum to n,
 * then this function computes the new lseq for n + 1.
 * 
 * @param   { unsigned int }  lseq  The current bit vector that represents the used Leonardo numbers.
 * @return  { unsigned int }        The new bit vector for the Leonardo numbers that sum to n + 1.
*/
unsigned int _SmoothSort_computeLseqIncrement(unsigned int lseq) {
  
  // Stores the rightmost bit of lseq
  // Gives us the index of the smallest used Leonardo number
  unsigned int lfirst = lseq & -lseq;

  // If the two smallest Leonardo's in the sequence are adjacent
  if(lseq & (lfirst << 1)) {

    // Appends the next Leonardo number
    lseq |= lfirst << 2;
    lseq &= lseq - 1;
    lseq &= lseq - 1;

  // If the two last bits are 0
  // Adds L1
  } else if(lseq % 4 == 0) {
    lseq += 2;

  // Adds L0
  } else if(lseq % 4 == 2) {
    lseq += 1;
  }

  // Return lseq
  return lseq;
}

/**
 * For a given integer n, if lseq represents the members of the Leonardo sequence that sum to n,
 * then this function computes the new lseq for n - 1.
 * 
 * @param   { unsigned int }  lseq  The current bit vector that represents the used Leonardo numbers.
 * @return  { unsigned int }        The new bit vector for the Leonardo numbers that sum to n - 1.
*/
unsigned int _SmoothSort_computeLseqDecrement(unsigned int lseq) {
  
  // Stores the rightmost bit of lseq
  // Gives us the index of the smallest used Leonardo number
  unsigned int lfirst = lseq & -lseq;

  // If we have a pair of 1's in our Leonardo seq, remove L0
  if(lseq % 4 == 3) {
    lseq -= 1;
  
  // Remove the remaining 1 if it's there
  } else if(lseq % 4 == 2) {
    lseq -= 2;
  
  // Otherwise, we get the smallest Leonardo and split it into two smaller ones
  // It's only in this conditional that we call the sift function because when 
  // singleton nodes are removed (Leonardo heaps of order 1), the overall structure
  // of the previously heapified data is undisturbed
  } else {

    // This implies that we've cut open a Leonardo tree and exposed two new heaps
    lseq &= lseq - 1;
    lseq |= (lfirst >> 1);
    lseq |= (lfirst >> 2);
  }

  // Return lseq
  return lseq;
}

/**
 * The main difference between smoort sort and heap sort is that smooth sort keeps the largest element 
 * in its correct place to begin with (no need for swapping it). 
 * 
 * Additionally, smooth sort uses a *forest* of heaps, and not just one. These heaps have the useful 
 * property wherein popping a root node off of any of them produces two new heaps that respect the 
 * valid shapes allowed in the forest of heaps. In this case, we're using the Leonardo numbers to 
 * define heap sizes, and because L(k) = L(k - 1) + L(k - 2) + 1, removing the root from any Leonardo 
 * heap of order k (which means we subtract 1 node) produces two new heaps of the next two smaller 
 * orders.
 * 
 * These heaps are ordered such that their roots are in ascending order. Rebalancing this structure
 * should be easy because performing the heapify operation only becomes necessary when the original
 * array is really unsorted. Otherwise, when it is near-sorted, we merely need to reorganize the roots
 * of the trees, which takes way less time than heapifying.
 * 
 * Performs smooth sort on the given array of records.
 * 
 * @param   { SmoothSort }  this      The smooth sort data object.
 * @param   { t_Record }    records   The array of records.
 * @param   { int }         n         The size of the array.
*/
void SmoothSort_main(SmoothSort this, t_Record records, int n) {
  int i;

  // We use the bits of the following unsigned int to indicate which Leonardo numbers are in use
  unsigned int lseq = 0, lfirst;
  
  // These just hold bits and other info for us
  int exp, offset;

  // Temp variable
  t_Record r = calloc(1, this.sizer());

  // We first build the forest of max-heaps with the Leonardo numbers
  for(i = 0; i < n; i++) {
    
    // Compute the new value of lseq for the current i
    lseq = _SmoothSort_computeLseqIncrement(lseq);
    lfirst = lseq & -lseq;

    // Get the smallest Leonardo heap size
    exp = (int) (log(lfirst) / log(2));
    offset = _SmoothSort_leonardoGet(this, exp);

    // Sift down the new roots if theyre not singletons
    if(lfirst > 2) {
      
      // Grab the root
      this.copier(r, records, 0, i);

      // Sift down
      _SmoothSort_siftDown(this, records, n, r, exp, i - offset, i);
    }
  }

  // Set some important stuff
  offset = 0;

  // We go through each of the heaps in reverse and sort the roots in ascending order
  // The iterator here is for traversing all the bits in a unsigned int
  for(i = sizeof(int) * 8 - 1; --i >= 0;) {
    lfirst = (lseq >> i) % 2;

    // We don't have a heap of that size
    if(!lfirst)
      continue;

    // The exponent here is actually just i
    exp = i;
    
    // Update the offset
    offset += _SmoothSort_leonardoGet(this, exp);

    // Do insertion only if we're not inserting the 0th element
    _SmoothSort_insert(this, records, n, offset - 1, (lseq >> exp) << exp);
  }

  // Finally, after generating the correct forest, we sort the sht
  for(i = n; --i >= 0;) {

    // Compute the first digit before modifying lseq
    lfirst = lseq & -lseq;

    // Compute the new lseq for the current i
    lseq = _SmoothSort_computeLseqDecrement(lseq);

    // If the smallest Leonardo heap that we broke wasn't a singleton;
    // In that case, we have to "reinsert" the new root nodes from the opened trees into the heapified area
    if(lfirst > 3) {

      // Get the smallest Leonardo heap size
      exp = (int) (log(lfirst >> 2) / log(2));
      offset = _SmoothSort_leonardoGet(this, exp);

      // We call it once for the left tree's root...
      // (during this iteration, we pass a version of lseq without the smallest Leonardo)
      _SmoothSort_insert(this, records, n, i - 1 - offset, lseq & ~(lfirst >> 2));

      // ...and another time for the right tree's root    
      _SmoothSort_insert(this, records, n, i - 1, lseq);
    }
  }

  // Free the temp
  free(r);
}

#endif