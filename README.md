# 1. An Analysis of Sorting Algorithms
#### *by Mo David*

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

# 4. Shuffling, Entropy, and Correlation

### 4.1. Shuffling

There is a well-known shuffling algorithm that generates a permutation of the elements of a sequence in a *reliably-random* way. By this, we mean to say that every possible permutation is equally likely, and the shuffling process does not favor any single one. This is called the ***Fisher-Yates algorithm***.

It's actually simpler than it sounds: you traverse an array of elements starting from the last element. At every element $k$, you randomly choose a new index $k'$ within the range $[0, k]$ and perform a swap with those two indices. You do this until you reach the start of the array. In pseudocode:

```Python
for k = (array.length - 1) to k = 0:
    k' = random number from 0 to k
    swap array[k] with array[k']
```

Despite it's simplicity, it reliably selects a permutation of the original array in a uniform manner (assuming your random function for choosing a number between each $[0, k]$ is unbiased).

To perform shuffling for this project, a slight variation of the original algorithm is used. Whereas the original Fisher-Yates always executes a shuffle for each element, the variation used here only performs swaps probabilitically. A swap happens $p$ amount of the time ($p=1$ means we always swap, $p=0$ means we never swap). In pseudocode:

```Python
for k = (array.length - 1) to k = 0:
    k' = random number from 0 to k
    t = random float from 0 to 1

    if t <= p and t > 0:
      swap array[k] with array[k']
```

In the implementation, things are notated a bit differently and we have $<$ instead of $<=$, but that's okay because the computations in the actual code account for that subtlety.

When performing a shuffle on data, it's helpful to know just how much of a shuffle we were able to do. To help us measure this idea of "shuffling", we come up with two metrics, the first of which is entropy.

### 4.2. Entropy

Entropy has analogues in physics and other aspects of life, and rightfully so. The concept of "disorder" is not too far from the concept of "messing up" a deck of cards, although context necessitates that we refer to the second case as "information entropy". Interestingly enough, both information theory and statistical mechanics have characterized entropy in much the same way. We focus on Claude Shannon's formulation in information theory (who impressively came up with this form [independent of any knowledge of statistical mechanics](https://mathoverflow.net/questions/403036/john-von-neumanns-remark-on-entropy)):

$$
\begin{align}
E = \sum_{k \in S} p_k \cdot -ln(p_k)
\end{align}
$$

gives the entropy of a discrete random variable $X$, where for some $k$ in the sample space $S$, $p_k$ is $P(X=k)$. The important thing to see here is that we're multiplying the probabilities of each of the outcomes in the sample space by how "surprising" they are. That's all $-ln(p_k)$ is telling us, because:

$$
\begin{align}
& -ln(p_k) \\
& =ln(\frac{1}{p_k})
\end{align}
$$

When $p_k=1$, then the event isn't suprising at all and the value above reduces to $0$. On the other hand, for smaller values of $p_k$, the value above increases and indicates that the event is "more surprising". Vaguely speaking maximizing entropy means "maximizing surprise", and this is achieved by dispersing our random variable across all possible values of the sample space. If some event were highly likely, then most of the time the outcome wouldn't surprise us at all. But if every outcome were equally likely, then each outcome would be very "surprising" because we would have no idea which one would likely happen next (as a fair warning, don't take the word "surprising" to mean anything rigorous here; it's only meant to be an analogy).

So how can we use this to analyze the "shuffledness" of our arrays? In our case, we choose select the difference between adjacent records to be the random variable whose entropy we will measure. This apparently provides a great way of measuring the entropy of an array of elements.

<!--! continue this part and explain better  -->

### 4.3. Correlation

# 5. Comparison and Individual Analyses

### 5.1 Comparisons

### 5.2 Analyses of Each Algorithm

# 6. Author

```
                                                    |\      _,,,---,,_
                                             ZZZzz /,`.-'`'    -.  ;-;;,_
                                                  |,4-  ) )-,_. ,\ (  `'-'
                                                 '---''(_/--'  `-'\_)
```