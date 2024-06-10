# 1. An Analysis of Sorting Algorithms

This repository currently only features a few select sorting algorithms. They are:

1. Insertion Sort
2. Selection Sort
3. Merge Sort
4. Heap Sort
5. Smooth Sort

In hindsight Smooth Sort was the most difficult to implement, but it also brought the me most enjoyment. Because the implementations of the other algorithms are already widely-known, I put emphasis on explaining the mechanism of Smooth Sort below and leave out the others (except for Heap Sort... It'll become clear why I decided to explain this too).

Additionally, I'll outline the methods I used to benchmark the sorting algorithms. After much research, I decided to generate my own data sets (still using the `struct Record` provided by the starter code) to test the different implementations. I landed on a few ways to measure "entropy" (how shuffled an array is) to help me understand how the sorting algorithms would behave under different circumstances.

Likewise, to verify the amount by which the different algorithms would scale (in terms of execution time), the sizes of the custom data sets were varied too.

# 2. Heap Sort

# 3. Smooth Sort

# 4. Shuffling, Entropy, and Sorting

### 4.1. Shuffling

There is a well-known shuffling algorithm that generates a permutation of the elements of a sequence in a *reliably-random* way. By this, we mean to say that every possible permutation is equally likely, and the shuffling process does not favor any single one. This is called the ***Fisher-Yates algorithm***.

It's actually simpler than it sounds: you traverse an array of elements starting from the last element. At every element $k$, you randomly choose a new index $k'$ within the range $[0, k]$ and perform a swap with those two indices. You do this until you reach the start of the array. In pseudocode:

```Python
for k = array.length to k = 0:
    k' = random number from 0 to k
    swap array[k] with array[k']
```

Despite it's simplicity, it reliably selects a permutation of the original array in a uniform manner (assuming your random function for choosing a number between each $[0, k]$ is unbiased).

To perform shuffling for this project, a slight variation of the original algorithm is used. Whereas the original Fisher-Yates always executes a shuffle for each element, the variation used here only performs swaps probabilitically. A swap happens $p$ amount of the time ($p=1$ means we always swap, $p=0$ means we never swap). In pseudocode:

```Python
for k = array.length to k = 0:
    k' = random number from 0 to k
    t = random float from 0 to 1

    if t <= p and t > 0:
      swap array[k] with array[k']
```

In the code, things are notated a bit differently and we have $<$ instead of $<=$, but that's okay because the computations in the actual implementation account for that subtlety.

### 4.2. Entropy

### 4.3. Sorting


# 5. Author

Mo David