<!-- 
PLEASE READ THE HTML FILE INSTEAD.
PLEASE READ THE HTML FILE INSTEAD.
PLEASE READ THE HTML FILE INSTEAD.
 -->

![an analysis of sorting algorithms](./README/title.png)

![overview](./README/headers/header-overview.png)
---

<br />

This repository currently only features a few select sorting algorithms. They are:

1. Insertion Sort
2. Selection Sort
3. Merge Sort
4. Heap Sort
5. Smooth Sort
6. Tim Sort

In hindsight, smooth sort was the most difficult to implement, but it also brought me the most insights and enjoyment. Because the implementations of the other algorithms are already widely-known, I place an emphasis on explaining the mechanism of smooth sort below and combine the others into a single section: ***Other Algorithms***. However, I've elected to devote a separate section for Heap Sort; this precedes my discussion of smooth sort (it'll become clear why I decided to explain this too).

The following report will also outline the methods that were used to benchmark the sorting algorithms. After much research, I decided to procedurally generate custom data sets (still using the `struct Record` provided by the starter code). These were created using a number of parameters (particularly, the number of records ($N$) and the shuffle amount ($P$), which we elaborate further in the sections below), and the integrity of the resulting shuffles were measured using well-defined metrics (in this case, entropy, correlation ($r$), and determination($r^2$)—again, more on these later). All in all, the parameters and measurements provided a richer context for analyzing the behavior of the algorithms under different circumstances. In line with the specifications of the project, both the execution times ($t$) and frequency counts ($f$) were recorded.

<br />

### 1.1 Running the Program

Compiling the program is relatively straightforward. Just use `gcc` and type the command `gcc main.c -o main`. The resulting executable `main` can then be run. **For linux users**, I do leave another small but important note as the math library is not linked by default: if you are using a Linux device, run `gcc main.c -o main -lm` instead.

Depending on what exactly you want to run, the program accepts a number of different command-line arguments. They are listed below.

* `algos`, the algorithms to run, default is `insertion,selection,heap,merge,smooth,tim`.
* `N`, the number of records to sort, default is `100`.
* `P`, the shuffling amount, default is `1.0`.
* `cycles`, the number of cycles per run, default is `1`.
* `runs`, the number of runs per $(N, P)$, default is `1`.
* `out`, the name of the file to write the results to, default is `output.csv`; of course, you may specify directories within the path.

So for example you could run the executable with the command `main algos=smooth,heap N=500,5000,50000 P=-1.0,-0.5,0.5,1.0 cycles=5 runs=2`. This performs two runs for each pair of $(N,P)$, with each run having $5$ cycles (if any of these terms seem vague or unclear, do not fret as these will be defined later on; for immediate information on them, jump to the sections on ***Shuffling, Entropy, and Correlation*** and ***Comparisons and Individual Analyses***). You can leave out *any or all* of the arguments above, and the program will run with the default values assigned to the unspecified arguments. The order of the arguments does not matter; that is, calling `./main N=1000,2000 P=1.0` and `./main P=1.0 N=2000,1000` would run the same routines. However, note that **strictly no spaces** are allowed when inputting the comma-separated values.

If you want to run the algorithms with the default datasets included in the starter code, you can also specify the `debug` argument; instead of calling `./main`, you run `./main debug`. The resulting process will execute each of the algorithms ten times for all of the starter datasets. The output of this process is not saved to a file, although the printed text can be piped into one if this is desired.

Note that for all tests, the integrity of the sorted arrays are automatically verified. It is ensured that **(1)** they are in the right order and **(2)** they have no missing entries. For more information on the specific implementation of these, head over to ***Comparisons and Individual Analyses***.

<br />

### 1.2 Creating Custom Tests

Because of the way the program is structured, it is possible to import `sortingalgorithms.c` into your own main function and test each of the following functions with your own data:

1. `Record_insertionSort(Record *records, int n)`
2. `Record_selectionSort(Record *records, int n)`
3. `Record_heapSort(Record *records, int n)`
4. `Record_mergeSort(Record *records, int n)`
5. `Record_smoothSort(Record *records, int n)`
6. `Record_timSort(Record *records, int n)`

Note how all the functions preserve the same signature to facilitate testing. Functions that use recursion (such as merge sort) have been abstracted into their own files to avoid succumbing to spaghetti code.

To gather the frequency counts on your own, simply call the corresponding `Record_get<sorter>SortFreq()` function. For instance, after calling `Record_heapSort(records, N)` for some variables `records` and `N`, you can then do something like `printf("heap sort frequency count: %ld", Record_getHeapSortFreq())`. The frequency count value is always of type `long`. 

> <br/>
> <b style="color: rgba(255, 55, 55, 1); background-color: rgba(255, 55, 55, 0.16); padding: 4px 8px;">IMPORTANT REMINDERS</b>
>
> Please keep the following in mind to avoid any problems running your custom tests:
>
> 1. When importing `sortingalgorithms.c` and using any of the `Record_<sorter>Sort()` functions, <b style="color: rgba(255, 55, 55, 1);">DO NOT FORGET TO RUN</b> `Record_initSorters()` at the start of the main function (with no parameters). This function has to be run once before any of the sorting algorithms can work.
> 2. <b style="color: rgba(255, 55, 55, 1);">DO NOT MOVE</b> any of the files around to prevent breaking any of the imports.
> 
> <br/>

<br />

### 1.3 A Note on Python Helper Files

You might notice that the project contains two Python files. These can be ignored and were only used to facilitate with the data collection and presentation. The first file `driver.py` allowed me to do batch tests without having to type a lot of different commands (this was necessary for my device because combining all tests into a single process ran the C executable for more than a few minutes, which caused it to be killed by the OS midway the test). The other Python script `grapher.py` was utilized to generate some of the visuals for this report (particularly those that have graphed data; the other illustrations were created in [Figma](https://figma.com/)).

![heap sort](./README/headers/header-heap-sort.png)
---

<br />

Heap sort has been described as ["selection sort using the right data structure"](https://link.springer.com/chapter/10.1007/978-3-030-54256-6_4). While this was not something that made sense to me a year ago, it was something that eventually clicked during this project. Both heap and selection sort choose the smallest (or largest) element within an array and move it to its proper location; however, heap sort takes $\mathcal{O}(\log n)$ time to perform this operation while selection sort takes $\mathcal{O}(n)$.

Heap sort treats the array differently: instead of viewing it as a sequential list of elements, heap sort visualizes it in the form of a tree, with the associations between parent nodes and child nodes outlined in a simple manner. Every $i^\text{th}$ element in the array is the child of the $\lfloor \frac{n - 1}{2} \rfloor^\text{th}$ element while also being the parent of elements $2i + 1$ and $2i + 2$. By defining the tree in this way, adjacent nodes can be found easily at the expense of just a multiplication or two.

<br />

### 2.1 Why Do We Use A Tree?

But what benefit does a tree have over the sequential representation of an array? Because of the structure of a tree (which in the case of heap sort is actually a ***max-heap***), we are guaranteed to know that every element is greater than all its descendants. This invariant allows us to shift an element to its right place within the structure without having to compare it to every single element in the array; the motion associated with performing this action is vertical along the tree, and thus only depends on the height of the tree. This is wonderful because the height of a binary tree with $n$ nodes is always bounded by $log_2(n) + 1$! Thus, visualizing the array in this manner allows us to execute a sort with $\mathcal{O}(\log n)$ comparisons per pass (giving us a total runtime of $\mathcal{O}(n \log n)$).

<br />

### 2.2 The Heap Sort Algorithm

As a brief outline of the actual heap sort algorithm (which I believe we must recall for the sake of understanding smooth sort), I present the following pseudocode:

```python
# The first step involves converting the array into a valid max-heap
# This operation takes nlogn steps.
for i = (array.length - 1) to i = 0:

    # Consider the subtree rooted at i, and sift the root down
    # Because we're starting with the leaves of the tree, this guarantees a valid max-heap at the end
    siftDown(array[i])

# The second step is the actual sorting
# We consider slices of the original array in decreasing length
# Again, this entire operation takes nlogn
for i = (array.length - 1) to i = 0:

    # Swap the largest element and bring it to the end of the current subarray
    swap(0, i);

    # Sift the new root of the max-heap
    # Since the max-heap was originally valid, we only need to sift the root to fix it
    siftDown(array[0]);
```

All in all, given that the two loops each take $\mathcal{O}(n \log n)$ time, executing them one after the other must also take $\mathcal{O}(n \log n)$.

It is also possible to optimize heap sort by starting the heapifying process at `i = pow(2, (int) log2(array.length)) - 1` (this is just the smallest power of $2$ less than the array length); this makes sense because the leaf nodes do not have children, and it would be pointless to call the function `siftDown()` on them. Nevertheless, even though the version of heap sort used in this project does not have this optimization, **heap sort was generally the fastest among the six sorting algorithms** that were chosen (at least according to the implementations of this project). For more information about the analyses and results, refer to the section on ***Comparisons and Individual Analyses***.

One important thing to note was that a different optimization was used to speed up heap sort. Originally, I managed to implement the algorithm exclusively using ***swaps***. When sifting elements down the tree, a swap would occur at every level when it was possible. This meant copying two different records unto two different locations. However, I later decided to defer copying the root until the sifting routine had found its final location within the heap: we could store the root in a temp variable and "shift" child nodes up instead of swapping them each time (much like insertion sort shifts adjacent elements rather than performing swaps). Following this modification, heap sort ran about twice as fast as it did with the initial implementation. This makes sense given that the amount of data that needed to be copied around was cut in half.

Anyway, on to the fun part...

![smooth sort](./README/headers/header-smooth-sort.png)
---

<br />

The reason I decided to explain heap sort prior to smooth sort was because the two algorithms rely on the same fundamental idea: they both visualize an array in a way that differs from its linear structure. The difference is that smooth sort attempts to remedy a certain "problem" with heap sort: the largest element of the array is always at the root (the beginning) of the array, when ultimately it must end up at the opposite end. This means that regardless of the initial state of our array, around $n \cdot \log n$ operations must necessarily happen.

Smooth sort, on the other hand, takes an unorthodox approach. For one, it doesn't create a single tree but rather a ***forest of max-heaps***. For another, it builds these trees such that their roots are located on the right. Believe it or not, this structure entails way less operations for arrays that are close to being sorted. It also means that smooth sort, in the best case, is $\mathcal{O}(n)$! Note that as amazing as this sounds, we will see towards the latter half of this paper (in the section ***Comparisons and Individual Analyses***) whether or not these merits are of any practical value.

But how does it work, exactly?

<br />

### 3.1 Leonardo Numbers and Leonardo Heaps

In order to discuss the underlying structure of the max-heaps created by smooth sort, I wish to bring forth the idea of [the Leonardo numbers](https://en.wikipedia.org/wiki/Leonardo_number). The Leonardo numbers are just an integer sequence defined by the recurrence:

<br/>

$$
\begin{align}
L(0) &= 1 \\
L(1) &= 1 \\
L(i) &= L(i - 1) + L(i - 2) + 1 \\
\end{align}
$$

<br/>

As a reference, I list the first few such numbers here, in the usual notation we define a sequence:

<br/>

$$
\begin{align}
L:\mathbb{N}_0 \rightarrow \{1, 1, 3, 5, 9, 15, 25, 41, 67, 109 \dots \}
\end{align}
$$

<br/>

To the acute reader, the recursive definition seems like an analogue of the Fibonacci sequence, aside from the $+1$ embedded into each iteration. Indeed, the two sequences are actually related by a simple formula, and this relationship is essential to proving the $\mathcal{O}(n \log n)$ behavior of smooth sort. But as neat as this may be, we avoid delving beyond the scope of this report (although you may ask me in person if you wish).

Now consider for a moment trees with $L(i)$ nodes. If we structure the trees such that the left subtree contains $L(i - 1)$ nodes and the right subtree contains $L(i - 2)$ nodes, then the trees have the amazing property of being recursive! Equivalently, given any two trees with $L(i - 1)$ and $L(i - 2)$ nodes respectively, a new tree with $L(i)$ nodes can be constructed from the other two by adding a single node and assigning this as the parent of the roots of the two subtrees. Because every node within any such graph has at most two children, these trees are binary trees. From hereon, we adopt the terminology used by [this article](https://www.keithschwarz.com/smoothsort/) and refer to these trees as ***Leonardo trees***. Given a Leonardo tree with $L(k)$ nodes, we define its ***order to be*** $k$.

![leonardo-trees](./README/figures/leonardo-trees.png)

Whereas heap sort uses a single complete binary tree to represent the array, smooth sort uses a forest of Leonardo trees instead. 

<br />

### 3.2 Generating the Forest

Generating the forest of Leonardo trees for smooth sort is more straightforward than it sounds. The process relies on the fact that any number can be expressed as the sum of a subset of the Leonardo numbers. We can prove this fact using mathematical induction, though again, we refuse to wander beyond the scope of this report.

We begin by starting at the left of the array and proceed rightwards until we reach the last element. During each iteration, the current element is either **(1)** added as a new tree on its own (a singleton), or **(2)** added as the root of the new tree built from the two rightmost Leonardo trees in the forest. In the second case, the new tree is heapified to ensure it is a valid max-heap and a `siftDown()` function is called (note that like heap sort, the `siftDown()` we use for smooth sort doesn't rely on swaps but shifts). We can see how this progresses below, where the newly added node is always highlighted.

![Smoothsort first stage.](./README/figures/smooth-sort-heapification.png)

The exact rules for deciding whether or not to append the next element as a root node or as a singleton are simple: if the two rightmost Leonardo trees in the forest have adjacent orders (one is of order $k$ and the other of order $k - 1$), then we merge them (alongside the new root) to create a Leonardo tree of the next order ($k + 1$). Otherwise, we add a singleton node. The actual code uses the bits of an `unsigned int` to keep track of the orders of the Leonardo trees currently in the forest: a $1$ on the $k^\text{th}$ least-significant bit (LSB) of the variable signifies that a Leonardo tree of order $k$ is present. Do note that because of this method, we are restricted to sorting arrays that hold a number of elements no more than the sum of the first $32$ Leonardo numbers (that's a few million records; if we want to circumvent this, we can just use an `unsigned long`). For the purposes of this project, this is more than enough.

<br />

### 3.3 Sorting the Roots of the Heapified Trees

Before we can proceed to doing the sort using the forest of Leonardo trees, we need to cover another subroutine of smooth sort. After it generates the forest of max-heaps, it must guarantee that the root of the rightmost tree is the greatest element in the array. This allows it to remove that node from the forest and place it into the sorted portion of the array (which is where it should've been already). But for this to be possible, the roots of all the trees in the forest must be sorted in ascending order. Sorting the roots of the Leonardo trees represents the second intermediate stage of the algorithm.

![Smoothsort second stage.](./README/figures/smooth-sort-root-sorting.png)

We are essentially performing insertion sort on each of the roots starting from the leftmost tree. At the end of each pass, the last modified tree is heapified (and the rest are left as is: if the root of a max-heap is replaced by a larger element, the max-heap property must still be satisfied by that tree). Eventually, the roots will be in ascending order.

<br />

### 3.4 Sorting Using the Heapified Trees

Once the roots have been sorted, we can begin removing the greatest element of the forest until the array is fully sorted. Since the greatest element must always be the root of the rightmost tree, there is actually no need to move this element; rather, most of the processing during this stage comes from fixing the structure of the remaining heaps.

During each root removal, either one of two things will happen: **(1)** the root leaves behind no subtrees because it was a singleton, in which case we can proceed to remove the next root, or **(2)** the root exposes two new subtrees, the roots of which must be rearranged with the roots of the existing trees on their left. In the second case, we repeat the process illustrated by the previous subsection. This fixes the roots of the forest and keeps them in ascending order, except this time we only need to shift the roots of the exposed subtrees (the rest of the roots should still be in order). Eventually, we reach the end of the array and arrive at a sorted list.

Note that for each iteration, performing insertion sort on the new roots (if there are any) will take at most $\mathcal{O}(\log n)$ time because every array of length $n$ can be turned into around $\log_\varphi n$ Leonardo trees. Heapifying the modified trees at the end of this insertion also takes $\mathcal{O}(\log n)$. We can add these together (since the operations happen one after the other), and overall, each pass still takes $\mathcal{O}(\log n)$. If we multiply this by the number of iterations required, we get $\mathcal{O}(n \log n)$. Adding this with the time complexity of the first stage of smooth sort still yields $\mathcal{O}(n \log n)$.

No illustrations exist for this part of the algorithm, as the other previous illustrations should suffice. Follow the first stage of smooth sort in reverse, and it should be easy to visualize how root removal looks; repeat the process outlined by the second stage, and understanding how each iteration sorts new roots should be trivial. 

<br />

### 3.5 But Why Leonardo Trees? Why Not Binary Trees?

Aren't binary trees recursive too? If you think about it, why use Leonardo trees at all—it's actually possible to perform smooth sort in the manner outlined above but with binary trees! Well, [Dijkstra puts forth a valid reason](https://citeseerx.ist.psu.edu/document?repid=rep1&type=pdf&doi=bec9f897d701a8326ccdfc97064f4ce2c07a732f): using Leonardo trees in place of binary trees incurs a speedup by a factor of about $1.2559$; while he likely left the derivation of this constant as an exercise to the reader, we take his word for it and implement the algorithm with Leonardo trees.

On the other hand, I leave it as an exercise to the reader to understand how smooth sort approaches linear time for nearly-sorted lists... (or you can ask me in person).

![other algorithms](./README/headers/header-other-algorithms.png)
---

<br />

### 4.1 Insertion Sort and Selection Sort

Insertion and selection sort are both $\mathcal{O}(n^2)$ algorithms. However, they have a few nuances that make one better than the other in certain situations.

As a general overview, selection sort "selects" the smallest (or greatest) element left in the ***unsorted portion** of the array*. Insertion sort "inserts" the current element into its correct location within the ***sorted portion** of the array*.

As a consequence of their implementations, selection sort always performs the same number of comparisons regardless of the initial state of the array. This implies that given a sorted array, selection sort won't even know its sorted until after comparing every possible pair of elements together. Insertion sort, on the other hand, can run in $\mathcal{O}(n)$ time in the best case. Because it only ever compares elements that are adjacent to each other, its conditional checks can end up "terminating" passes prematurely when the array is already sorted. However, because of the excessive number of memory calls required by insertion sort (shifting elements takes way more time than just comparing two values), it tends to be slower than selection sort.

<br />

### 4.2 Merge Sort

Merge sort takes a divide-and-conquer approach to sorting an array. Given any array, the merge sort routine splits it into two new arrays of half the size and calls itself on those arrays. Eventually, arrays of size 1 will be left; these arrays are considered sorted by default. When sorted arrays are encountered, merging can then occur. Merges happen by repeatedly selecting the smaller of the leftmost elements in both arrays and pushing this unto the sorted array (note that the "sorted array" here refers to an auxiliary piece of memory; other implementations of merge sort exist with less than $\mathcal{O}(n)$ space consumption, although they come with the cost of increasing the time complexity of the algorithm). At some point, all the inner function calls resolve into even larger subarrays, and a sorted version of the array is gradually created. The final step of the algorithm (according to the implementation of this project) involves copying the sorted version of the array onto the original.

Merge sort has a time complexity of $\mathcal{O}(n \log n)$. This is both its best case and its worst case. The reason for this is that there will be at most $\log_2(n)$ levels of divisions of an array into halves; in other words, the deepest level of a nested merge sort function call will be at most $\log_2(n)$. And for each level, the total number of operations executed by all nested calls will be on the order of $\mathcal{O}(n)$, because we're traversing each subarray linearly during the merges.

<br />

### 4.3 Tim Sort

Now I won't bother going in-depth with tim sort; it's not really the main algorithm I chose anyway. Nevertheless, I feel like it deserves a special mention. The original publication outlining tim sort actually takes inspiration [from another academic paper](https://dl.acm.org/doi/10.5555/313559.313859) which led me down a rabbit hole of information theory. This paper eventually helped me realize my ideas on how to benchmark the sorting algorithms.

Some caveats with the implementation of tim sort used by this project: it's not adaptive, and it's not completely faithful to the original. For one, the run size doesn't change based on the number of records, although in practice it should adapt to try and minimize the number of resulting merges utilized by the algorithm. Aside from this, the algorithm also performs merges only after defining the runs, as opposed to performing them when merge criteria are satisfied. Nevertheless, contrary to these oversimplifications, I still refer to this version of the algorithm as tim sort, since a lot of the implementations found online follow this pattern (despite its apparent irreverence to the original).

![shuffling, entropy, and correlation](./README/headers/header-shuffling-entropy-correlation.png)
---

<br />

### 5.1 Shuffling

There is a well-known shuffling algorithm that generates a permutation of the elements of a sequence in a *reliably-random* way. By this, we mean to say that every possible permutation is equally likely, and the shuffling process does not favor any single one. This is called the [Fisher-Yates algorithm](https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle).

It's actually simpler than it sounds: you traverse an array of elements starting from the last element. At every element $k$, you randomly choose a new index $k'$ within the range $[0, k]$ and perform a swap with those two indices. You do this until you reach the start of the array. In pseudocode:

```Python
for k = (array.length - 1) to k = 0:
    k' = randomNumber(0, k)
    swap(array[k], array[k'])
```

Despite its simplicity, it reliably selects a permutation of the original array in a uniform manner (assuming your random number function is unbiased).

To conduct the array shuffling for this project, a slight variation of the original algorithm was used. Whereas the original Fisher-Yates always executes a shuffle for each element, the variation used here only performs swaps probabilistically. Swaps happen $P$ amount of the time ($P=1$ means we always swap, $P=0$ means we never swap). In pseudocode:

```Python
for k = (array.length - 1) to k = 0:
    k' = randomNumber(0, k)
    t = randomFloat(0, 1)

    if t <= P and t > 0:
        swap(array[k], array[k'])
```

In the implementation, things are notated a bit differently and we have $<$ instead of $<=$, but that's okay because the computations in the actual code account for that subtlety.

When performing a shuffle on data, it's helpful to know just *how much of a shuffle* we were able to do. To help us measure this idea of "shuffling", we come up with a few metrics, the first of which is entropy.

<br />

### 5.2 Entropy

Entropy is often associated with the idea of disorder. Fundamentally, the concept of "disorder" is not too far from the concept of "messing up" a deck of cards, although context necessitates that we refer to the second case as [information entropy](https://en.wikipedia.org/wiki/Entropy_(information_theory)#:~:text=Generally%2C%20information%20entropy%20is%20the,referred%20to%20as%20Shannon%20entropy.). Interestingly enough, both information theory and statistical mechanics have characterized their notions of entropy in much the same way. We focus on the formulation Claude Shannon provided for information theory (who impressively came up with the form [independent of any knowledge of statistical mechanics](https://mathoverflow.net/questions/403036/john-von-neumanns-remark-on-entropy)):

<br>

$$
\begin{align}
H(X) = -\sum_{x \in S} p_x \cdot ln(p_x)
\end{align}
$$

<br>

gives the entropy of a discrete random variable $X$, where for some $x$ in the sample space $S$, $p_x$ is $\text{I\kern-0.15em P}(X=x)$. The important thing to see here is that we're multiplying the probabilities of each of the outcomes in the sample space by how "surprising" they are. That's all $-ln(p_x)$ is telling us, because:

<br>

$$
\begin{align}
& -ln(p_x) \\
& =ln(\frac{1}{p_x})
\end{align}
$$

<br>

When $p_x=1$, then the event isn't suprising at all and the value above reduces to $0$. On the other hand, for smaller values of $p_x$, the value above increases and indicates that the event is "more surprising". Roughly speaking, maximizing entropy means "maximizing surprise and uncertainty", and this is achieved by dispersing our random variable across all possible values of the sample space. If some event were highly likely, then most of the time the outcome wouldn't surprise us at all. But if every outcome were equally likely, then each outcome would be very "surprising" because we would have no idea which one would likely happen next (as a fair warning, don't take the word "surprising" to mean anything rigorous here; it's only meant to be an analogy).

So how can we use this to analyze the "shuffledness" of our arrays? Entropy manifests in our sequences of records when it becomes hard to expect which record comes after a given record. In a sorted list (or a list with some structure to it), this is easy because the records form a sort of pattern. Thus, when traversing the array, it is reasonable to have an expectation for what the next record might be at any given point (it won't be anything *surprising*). However, when an array is shuffled really well, it becomes really hard to tell what record might come next, and so things are "uncertain" and "surprising". 

[To approach this more rigorously](https://stats.stackexchange.com/questions/78591/correlation-between-two-decks-of-cards/79552#79552), we define a random variable $X$ to represent the possible differences between any two adjacent records. That is, for any two adjacent records $r_i$ and $r_{i + 1}$ along the shuffled array, we define $x_i$ to be

<br>

$$
\begin{align}
x_i = s(r_{i + 1}) - s(r_{i})
\end{align}
$$

<br>

where $s(r_i)$ returns the index of the record *in the sorted array* (keep in mind that the $i$ here denotes the position of the record in the original array). Thus, a highly entropic array will have $X$ vary considerably, whereas in a sorted array, $P(X=1)$ is $1$ and no other differences occur.

Do note that in order for this approach to work, we adjust negative differences to fall within the range $[0, N-1]$ ($N$ is the number of records). We do this by adding $N$ whenever $x_i < 0$. Effectively, this just gives us the residue of $x_i \text{ mod } N$. If we were to leave this step out, our computation for entropy would likely have its terms cancel out. We would end up with a measure of $0$ when in fact the system displays a high degree of entropy.

<br />

### 5.3 Correlation and Determination

[Another useful idea](https://stats.stackexchange.com/questions/78591/correlation-between-two-decks-of-cards/) to help us assess the "shuffledness" of an array is correlation. Using the same function $s(r_i)$ from above, we can create a rough estimate of the disorder in our array by determining the strength of the correlation between $i$ and $s(r_i)$. A sorted array will always have $i=s(r_i)$ (that is, the values of the sorted indices of the records are equal to their current position in the array), and would produce a correlation of exactly $1$; a sorted array *in reverse order* would give $${-1}. An unsorted array will likely have a very small correlation value (something close to 0). Unlike entropy, our scale lives within the interval $[-1, 1]$.

However, when measuring shuffledness alone, the measure of correlation tends to be a bit superfluous in that we don't need to know whether or not the array was in reverse or the right order. Either way, the array wouldn't be considered "well-shuffled" (it might not be sorted, but it isn't that jumbled up). To give us a better sense of shuffledness alone, we can square the value of the correlation coefficient. This particular number is used a lot in inferential statistics and has a special name: [the coefficient of determination](https://en.wikipedia.org/wiki/Coefficient_of_determination). Although this number has a few different interpretations, we ignore most of them as they do not apply to our framework. Nevertheless, we use it to quantify the disorder of our datasets.

We will still use both the coefficient of correlation and determination to assess the performances of the different algorithms. The latter will supplement our measurements of entropy, while the former will give us insights into how the algorithms react to data that are nearly sorted or nearly reversed. From hereon, we refer to the two as $r$ and $r^2$, respectively.  

![comparisons and individual analyses](./README/headers/header-comparisons-analyses.png)
---

<br />

This section discusses the two different methods used to compare and analyze the different algorithms. The first uses the provided datasets for the project; there are seven of these, and all algorithms we're allowed to run on them ten times. The second involves a testing framework specifically coded for this project. Do note that flexibility was considered in designing this system, and the framework may be used to benchmark other sorting algorithms (even those that don't use the `Record` data structure) so long as they follow the interfaces required by the framework.

<br />

### 6.1 Preliminary Testing with the Starter Datasets: Time Taken by the Algorithms

This test was relatively straightforward. To ensure the reliability of the measured durations, each algorithm was run ten times on all of the provided datasets. The results were then recorded unto a text file (by piping the actual output of the executable) and are summarized in the table below. Note that all the values depicted here represent the average duration taken by the algorithm across their ten attempts (and thus contain an extra significant figure).

| Dataset / Algorithm   | Insertion Sort | Selection Sort | Heap Sort  | Merge Sort | Smooth Sort | Tim Sort   |
| --------------------- | -------------- | -------------- | ---------- | ---------- | ----------- | ---------- |
| `random100.txt`       | $0.6$ ms       | $0.3$ ms       | $0.4$ ms   | $0.4$ ms   | $0.5$ ms    | $0.2$ ms   |
| `random25000.txt`     | $8039.3$ ms    | $3617.0$ ms    | $39.9$ ms  | $61.5$ ms  | $56.3$ ms   | $48.7$ ms  |
| `random50000.txt`     | $39824.6$ ms   | $22959.9$ ms   | $93.6$ ms  | $149.8$ ms | $138.7$ ms  | $111.7$ ms |
| `random75000.txt`     | $97120.4$ ms   | $60394.9$ ms   | $148.8$ ms | $279.6$ ms | $241.7$ ms  | $237.7$ ms |
| `random100000.txt`    | $181901.7$ ms  | $110583.4$ ms  | $230.6$ ms | $402.0$ ms | $369.6$ ms  | $341.6$ ms |
| `almostsorted.txt`    | $33722.8$ ms   | $112857.2$ ms  | $190.6$ ms | $343.5$ ms | $120.8$ ms  | $303.4$ ms |
| `totallyreversed.txt` | $369691.3$ ms  | $129937.2$ ms  | $158.1$ ms | $394.1$ ms | $210.2$ ms  | $348.7$ ms |

As expected, both $\mathcal{O}(n^2)$ algorithms had runtimes that increased significantly with respect to the problem size. In the worst case scenario (sorting a list in reverse), insertion sort took around $6$ mins, while selection sort took a little over $2$ mins. While selection sort is supposed to run at about the same time for a fixed problem size (regardless of the shuffle) due to its constant number of comparisons for a given $N$, the slight increase in execution time observed for `totallyreversed.txt` is likely due to the elevated *number of swaps* performed by selection sort. On the other hand, when looking at the best case scenario (sorting the `almostsorted.txt` dataset), selection sort unfortunately does worse off for some reason. However, insertion sort finishes in about half a minute: this makes sense as insertion sort is expected to run closer to $\mathcal{O}(n)$ for nearly sorted datasets.

For the $\mathcal{O}(n \log n)$ algorithms, we make some interesting observations. Heap sort performs considerably better on data that's almost sorted, but performs even better on data that's sorted in reverse! Almost the same can be said for smooth sort, although it tends to prefer the correct sort order of data. Based on our discusion of entropy above, we know that the datasets in these cases should have low measures of disorder (a reversed array isnt shuffled that well, it's just in the opposite order), and as we will see later, heap sort and smooth sort perform considerably better for most datasets with low entropy. We will tackle why this is the case in the latter half of this section (when we discuss the custom testing framework that was used). For the other $\mathcal{O}(n \log n)$ algorithms, the improvements they exhibit for low-entropy datasets are far less pronounced. However, when dealing with highly entropic data, heap sort and tim sort tend to dominate in terms of execution time. 

<br />

### 6.2 Preliminary Testing with the Starter Datasets: Frequency Count of the Algorithms

For the simpler algorithms, measuring frequency counts was pretty straightforward. Algorithms like insertion sort and selection sort were the easiest to configure for this, since they had no subroutines. Merge sort, tim sort, and heap sort were slightly more difficult because of the recursions they employed, but they were still relatively manageable. The real difficulty came with smooth sort. Smooth sort used so many helper functions I had to decide which ones to include and which ones to regard as just "math operations". 

Initially, I came up with a decent rule of thumb: the counter would only be incremented *at the start of every loop and at the start of every function call*. But given the convoluted nature of smooth sort, that rule wasn't rigid enough. For instance, one of the loops used by smooth sort did nothing but iterate through the bits of a single `unsigned int` and performed some basic arithmetic on them. It didn't make sense to count the loop as a sequence of many different instructions because the loop as a whole just did a single math operation (or to put it another way, the math operation I was performing with a loop probably wouldn't use an explicit loop in other languages). In the end, I struck a compromise and made sure that **the functions and loops that were meant to *iterate through records* were counted**. This was how I defined the frequency counts for all the algorithms. 

| Dataset / Algorithm   | Insertion Sort  | Selection Sort  | Heap Sort   | Merge Sort  | Smooth Sort | Tim Sort    |
| --------------------- | --------------- | --------------- | ----------- | ----------- | ----------- | ----------- |
| `random100.txt`       | $2,856$         | $5,140$         | $878$       | $1,618$     | $1,414$     | $1,096$     |
| `random25000.txt`     | $156,009,090$   | $312,512,499$   | $418,922$   | $800,042$   | $842,653$   | $671,449$   |
| `random50000.txt`     | $625,794,731$   | $1,250,024,999$ | $887,555$   | $1,700,044$ | $1,807,629$ | $1,442,943$ |
| `random75000.txt`     | $1,411,869,612$ | $2,812,537,499$ | $1,373,838$ | $2,700,046$ | $2,824,141$ | $2,314,729$ |
| `random100000.txt`    | $2,489,326,458$ | $5,000,049,999$ | $1,875,310$ | $3,600,046$ | $3,860,019$ | $3,088,714$ |
| `almostsorted.txt`    | $572,622,325$   | $5,000,049,999$ | $1,936,069$ | $3,600,046$ | $1,659,917$ | $2,835,761$ |
| `totallyreversed.txt` | $5,000,049,999$ | $5,000,049,999$ | $1,797,435$ | $3,600,046$ | $3,284,188$ | $3,462,540$ |

As we've mentioned above, selection sort will perform the same number of iterations regardless of the state of the original array. This is clearly visible in the table above, where selection sort performs the same number of iterations on the datasets with $100000$ records. Merge sort has the same property, although do note that this does not mean it performs the same number of swaps regardless of the state of the array. With the way the frequency count was set up, it just means that it performs the same number of *iterations on the array*. 

When looking at the other sorting algorithms, we can see that they perform a varying number of iterations. Smooth sort seems to have the largest change in this: when dealing with arrays that are close to being sorted, smooth sort executes significantly less iterations. While heap sort had a similar property with regard to execution time, it does not seem to have much of an improvement when considering the frequency count produced by the algorithm. Again, we will tackle why this is the case in the proceeding sections.

<br />

### 6.3 The Custom Testing Framework: The Sort Checker

Before we proceed to the collected data, we must first discuss the methods that were used to verify the correctness of the sorted arrays. Initially, this was done by checking the order of the elements in the final array. If the array had its elements in a nondecreasing (or nonincreasing) arrangement, then the checker would say that the array was successfully sorted. However, after debugging the more convoluted algorithms, I encountered problems that made me realize this check was insufficient. Sometimes, due to logical bugs, certain entries would be duplicated and would overwrite other entries. It was then possible to have a sorted collection of elements but with some of the original entries missing from the final array.

To remedy this, I implemented a binary search that checked if every element in the original array was also present in the final array. If the first check was successful (the array was in the correct order), then the checker would proceed to execute the binary search. 

With both checks in place, this meant that the verification of the sortedness of the array took at most $\mathcal{O}(n \log n)$ time, since $n$ different binary searches of complexity $\mathcal{O}(\log n)$ would be conducted (the first check only took $\mathcal{O}(n)$ since it sufficed to compare just the adjacent elements within the array).

<br />

### 6.4 The Custom Testing Framework: Doing the Runs and Cycles

The custom testing framework executes a number of different ***runs***, each of which performs a set number of ***cycles***. In the context of the framework, each run defines *a specific **shuffle of records** for a given pair of values* $(N, P)$ (recall that we defined $N$ and $P$ in ***Shuffling, Entropy and Correlation***), a cycle ***runs the selected algorithms once** for the shuffle of the current run*. Multiple cycles ensure that we account for the actual time it takes each algorithm to sort a given shuffle (in case outliers of "bad timing" happen to be present); runs allow us to be confident that the times we're getting aren't for a particularly "good" or "bad" shuffle for the current $(N, P)$ (the shuffle wasn't an unlikely one). If this still isn't clear, the pseudocode below should illustrate what I mean:

```python

# This performs a test for a given N and a given P
set value of N
set value of P

suffle_data = 1d array storing information about shuffles   # init to all 0s
times = 2d array to store execution times                   # init to all 0s
freqs = 2d array to store freq counts                       # init to all 0s

# Testing algorithm for a given N and P
for run in runs:
    shuffle = generateNewShuffle(N, P)

    # This not only saves the current values of N and P
    # It also saves the measured entropy, correlation (r) and determination (r^2) of the current shuffle
    shuffle_data[run] = getShuffleParameters(shuffle)

    # Perform the required number of cycles
    for cycle in cycles:

        # Do the sort for each algorithm
        for algo in algorithms:
            tosort = createShuffleCopy(shuffle)
            
            start = getStartTime()
            algo.sort(tosort)
            end = getEndTime()
            
            # Get the measurements for this cycle
            t = end - start
            f = algo.freqCount

            times[algorithm][run] += t
            freqs[algorithm][run] += f
        
    # Get the average of all cycles for this run
    times[algorithm][run] /= cycles
    freqs[algorithm][run] /= cycles
```

Note that when we save the results of a run, we're also saving the values of $N$ and $P$ that were used by that run, as well as the measurements of entropy, $r$, and $r^2$ that were gathered for its shuffle (entropy, $r$, and $r^2$ are also defined in  ***Shuffling, Entropy and Correlation***).

For all the data collection that were performed using the framework, the number of cycles and runs were always set to $10$.

<br />

### 6.5 The Custom Testing Framework: Results and Analysis

<br />

| Execution Time Graphs                                                     | Frequency Count Graphs                                                    |
| ------------------------------------------------------------------------- | ------------------------------------------------------------------------- |
| ![](./README/data-graphs/all/time-all-lnt-vs-lnN,for-P=1.0.png) | ![](./README/data-graphs/all/freq-all-lnt-vs-lnN,for-P=1.0-no-leg.png) |

We begin with a general comparison of all the algorithms on one plot. From here, the asymptotic behavior of the different algorithms are obvious. The two red plots are those of the $\mathcal{O}(n^2)$ algorithms. The rest belong to the $\mathcal{O}(n \log n)$ algorithms. While our graphs of the execution times have a bit of variation (likely due to fluctuations in the device performance while testing), the graph of the frequency counts show a more rigid structure and support our theoretical understanding of the algorithms.

If we plot the performance of the algorithms against $P$ instead of $N$, we observe a different aspect of the algorithms:

<br />

| Execution Time Graphs                                                      | Frequency Count Graphs                                                      |
| -------------------------------------------------------------------------- | --------------------------------------------------------------------------- |
| ![](./README/data-graphs/all/time-all-t-vs-P,for-N=10000.png) | ![](./README/data-graphs/all/freq-all-t-vs-P,for-N=10000-no-leg.png) |

We can see that the quality of our graphs have severely degraded. Jokes aside, we start isolating the $\mathcal{O}(n \log n)$ algorithms from those of $\mathcal{O}(n^2)$ to be able to better compare them. Although the graphs above show us how insertion sort performs better on datasets that are close to sorted ($P>0$ but $P << 1$), the changes in the behavior of the $\mathcal{O}(n \log n)$ algorithms are much harder to see because of this. Now I believe that the more interesting analyses will come from our exploration of the $\mathcal{O}(n \log n)$ algorithms; because of this, we will not return to insertion and selection sort until near the end of this subsection.

<br />

| Execution Time Graphs                                                      | Frequency Count Graphs                                                      |
| -------------------------------------------------------------------------- | --------------------------------------------------------------------------- |
| ![](./README/data-graphs/nlogn/time-nlogn-t-vs-P,for-N=1000.png) | ![](./README/data-graphs/nlogn/freq-nlogn-t-vs-P,for-N=1000-no-leg.png) |
| ![](./README/data-graphs/nlogn/time-nlogn-t-vs-P,for-N=10000-no-leg.png) | ![](./README/data-graphs/nlogn/freq-nlogn-t-vs-P,for-N=10000-no-leg.png) |
| ![](./README/data-graphs/nlogn/time-nlogn-t-vs-P,for-N=25000-no-leg.png) | ![](./README/data-graphs/nlogn/freq-nlogn-t-vs-P,for-N=25000-no-leg.png) |

We can see just how much more nuance exists with these algorithms. It seems that all of them exhibit performance boosts for data that is nearly sorted, but none parallel the drastic gains flaunted by smooth sort. Especially when considering execution time alone, smooth sort becomes faster than heap sort for values of $P \in [0, 0.1)$. And even for $P$ in the range $[0, 0.5)$, smooth sort still bests the other two algorithms within its complexity class. 

When looking at the execution times for heap sort, it seems to prefer both small negative and positive values of $P$: for $P \in [0, 0.1)$ **and** $P \in (-0.1, 0]$, heap sort performs significantly better. The reason heap sort speeds up even for reversed data sets is because the heapification process used by heap sort *prefers data in reverse order*. This was the very "problem" smooth sort tried to remedy (which is why smooth sort doesn't seem to be able to display the same resilience for reversed data sets). Nevertheless, both algorithms still perform significantly better for nearly sorted datasets, with the most likely reason being that less restructuring of the heaps are necessary when sorting data that's already structured.

Now using the $P$ value alone isn't the most robust way to analyze the performance of the algorithms. It would be better to compare their performances with respect to the actual state of the shuffled array. In line with this, we first look at the performances of our algorithms with respect to the actual measured entropy of the different shuffles:

<br />

| Execution Time Graphs                                                      | Frequency Count Graphs                                                      |
| -------------------------------------------------------------------------- | --------------------------------------------------------------------------- |
| ![](./README/data-graphs/nlogn/time-heap-t-vs-entropy,for-N=25000.png) | ![](./README/data-graphs/nlogn/freq-heap-t-vs-entropy,for-N=25000-no-leg.png) |
| ![](./README/data-graphs/nlogn/time-merge-t-vs-entropy,for-N=25000.png) | ![](./README/data-graphs/nlogn/freq-merge-t-vs-entropy,for-N=25000-no-leg.png) |
| ![](./README/data-graphs/nlogn/time-smooth-t-vs-entropy,for-N=25000.png) | ![](./README/data-graphs/nlogn/freq-smooth-t-vs-entropy,for-N=25000-no-leg.png) |
| ![](./README/data-graphs/nlogn/time-tim-t-vs-entropy,for-N=25000.png) | ![](./README/data-graphs/nlogn/freq-tim-t-vs-entropy,for-N=25000-no-leg.png) |

We've now isolated each algorithm into its own plot; an aggregate of all of these graphs would've been incomprehensible. Here, we confirm that heap sort has a bias for datasets with low entropy; that is, there is a strong correlation between the entropy of a shuffle and the time taken by heap sort to order that shuffle. Merge sort and tim sort have slight trends in their own right, though they are not as prominent. Smooth sort, on the other hand, has a somewhat boomerang-like shape. This tells us that graphing the execution time of these functions with respect to entropy alone hides something important. And indeed, the frequency counts of tim and heap sort show the same pattern. If instead we graph the execution times of these algorithms with respect to the $r$ value associated with the shuffle, we get a more comprehensive picture of the situation. 

<br />

| Execution Time Graphs                                                      | Frequency Count Graphs                                                      |
| -------------------------------------------------------------------------- | --------------------------------------------------------------------------- |
| ![](./README/data-graphs/nlogn/time-heap-t-vs-correlation,for-N=25000.png) | ![](./README/data-graphs/nlogn/freq-heap-t-vs-correlation,for-N=25000-no-leg.png) |
| ![](./README/data-graphs/nlogn/time-merge-t-vs-correlation,for-N=25000.png) | ![](./README/data-graphs/nlogn/freq-merge-t-vs-correlation,for-N=25000-no-leg.png) |
| ![](./README/data-graphs/nlogn/time-smooth-t-vs-correlation,for-N=25000.png) | ![](./README/data-graphs/nlogn/freq-smooth-t-vs-correlation,for-N=25000-no-leg.png) |
| ![](./README/data-graphs/nlogn/time-tim-t-vs-correlation,for-N=25000.png) | ![](./README/data-graphs/nlogn/freq-tim-t-vs-correlation,for-N=25000-no-leg.png) |

The graphs look less like relations and more like functions, which is a good thing because we can infer much more from them now. The reason smooth sort had a boomerang-like graph when we were plotting against entropy was because entropy (as we discussed earlier) fails to account for the "direction" of the array (that is, whether or not it is kind of sorted or kind of reversed). By using $r$, we get to see the effect of this aspect of the shuffle on the different sorting algorithms.

Again, we confirm that smooth sort indeed behaves better for nearly-sorted arrays, and just a bit more when they're totally reversed. Heap sort still displays the same affinity for low-entropy shuffles (both sorted and reversed arrays) when looking at its execution time; however, its frequency count tends to get better for reversed arrays precisely because its initial heapification stage ends up ordering the data in reverse. The only reason heap sort also performs better for nearly-sorted arrays is because the existing structure within a sorted shuffle leads to less sift operations later on during the execution of the algorithm.  

But looking at merge sort, it seems as if the initial state of the array has little effect on its total execution time. This makes sense, as the number of recursive calls made by the routine will not be affected by the arrangement of the data; the same is true for the number of data comparisons and data copies made by merge sort.

Tim sort, on the other hand, seems to display a preference for sorted data and a complete aversion for reversed arrays. The best way to understand this is to think of tim sort as a hybrid of merge and insertion sort (this is actually what it is). With this in mind, although we know that merge sort maintains the same number of operations regardless of the shuffle, insertion sort performs better for arrays that are closer to being sorted. With that, we have the perfect segue to return to the $\mathcal{O}(n^2)$ algorithms.

<br />

| Execution Time Graphs                                                     | Frequency Count Graphs                                                           |
| ------------------------------------------------------------------------- | -------------------------------------------------------------------------------- |
| ![](./README/data-graphs/all/time-all-t-vs-correlation,for-N=500.png)  | ![](./README/data-graphs/all/freq-all-t-vs-correlation,for-N=500-no-leg.png)  |
| ![](./README/data-graphs/all/time-insertion-t-vs-correlation,for-N=500.png) | ![](./README/data-graphs/all/freq-insertion-t-vs-correlation,for-N=500-no-leg.png) |

We no longer show selection sort in isolation, because it has a dull straight horizontal line (it's very uninteresting). Insertion sort has a behavior similar to that of tim sort when analyzing its performance based on the $r$ value of the datasets. This greatly overshadows the changes exhibited by the other algorithms, mostly because going from $\mathcal{O}(n^2)$ to $\mathcal{O}(n)$ is a significantly greater shift than going from $\mathcal{O}(n \log n)$ to $\mathcal{O}(n)$. 

Note that our tests of the $\mathcal{O}(n^2)$ algorithms ran with considerably less records as the tests would've taken forever otherwise.

![recommendations, afterthoughts, and anecdotes](./README/headers/header-recommendations-afterthoughts-anecdotes.png)
---

<br />

Here are a few reflections I've had the privilege of noting down following the completion of the project. Note that these are not in chronological order.

<br />

### 7.1 Smooth Sort Bug: C Behaves Differently on Different Platforms

This is probably one of the funniest bugs I've ever encountered... it's even better because it didn't come from a problem with my code.

When I was implementing smooth sort, I found myself needing a way to compute the logarithm of a number with respect to base 2. This was necessary in order to read off the bits contained in a standard `unsigned int` (or rather, their indices) without the need for a loop. Initially, the solution I came up with used the `log()` function of the C library, which took logarithms in base $e$. Since I had no idea `log2()` existed, I computed my logarithms by doing the usual base switch: $log_2(x) = \frac{\log_b(x)}{\log_b(2)}$. I would do this a number of times throughout the smooth sort routine.

This was working all fine and well (which I guess one should expect since the formula is mathematically correct) until at some point I decided to run my program on Windows. Up until then I'd been working within a Linux environment, and I wasn't expecting to have to deal with platform differences because I wasn't touching anything that low-level. But for some reason, despite the same code running on both my devices, smooth sort failed to sort my arrays on Windows when it successfully did on Linux. Although I now know that the reason for this was the logarithm formula I used above, it took me an hour or two to debug the whole mess and isolate the problem. It's honestly a miracle I found the problem that fast, because my smooth sort implementation had so many components I could've just as well spent an entire day figuring that one out.

It turns out that doing `(int) log(8) / log(2)` on Windows gave... $2$! What? Why? I honestly don't know, but this was also true for other powers of two. The `log(x) / log(2)` would work fine for just about any value of $x$ **EXCEPT** for powers of two. And that really messed with smooth sort. Linux didn't have this problem, though, and it's likely it has something to do with the float-precision of the numbers being used. Regardless, I immediately found a solution to my problem (which in hindsight should've been the implemetation since the start): `log2()`. If only I'd known about the function at the beginning, the problem would've never occurred... but is it my fault? Perhaps, perhaps not. It's either bad documentation on the part of the C Library or bad documentation comprehension on my part.

<br />

### 7.2 Shifting, not Swapping

Initially, heap sort actually wasn't the fastest algorithm; there was no clear winner, but tim sort and merge sort seemed to dominate for the most part. That was until I realized all the algorithms that used swaps could be optimized. If I copied insertion sort and stored the current element in a temp variable, I could theoreitcally cut the execution times of the algorithms in half (as they would have to copy half the amount of data per shift vs. per swap). This actually ended up being true and sped up heap sort so much that it overtook every other algorithm after that. Do note that I did the same optimization for smooth sort, but it was already kind of slow to begin with, so even though it ran twice as fast, it was still slower than its $\mathcal{O}(n \log n)$ brethren (for the most part). Again, I suspect the excessive amount of function calls to be part of the reason why this is so (for smooth sort, I isolated so many subroutines into their own functions for convenience, but I think that might have ended up hurting performance). And actually, I was a bit right, because smooth sort gained another optimization after I did a certain refactoring...

<br />

### 7.3 I Should've Read the Specs First... But Also Touche, Good Software Design

I'd spent about ten days on the entire project at that point when suddenly, the day before the submission, I reread the specifications and realized I needed to do the frequency counts for each algorithm (I had no idea that was required). I was about to lose it when I realized I'd created structs for each of the sorting algorithms, so that storing state for each of them (such as frequency count) would be no biggie. The function signature would remain practically the same, and only an extra struct member would need to be created. While it did take about half an hour to set up (and I had to redo all the tests again), the fact that it took *only half an hour* to set up was a relief. I felt kinda happy realizing the way I code these days gives my programs a leeway for flexibility.

<br />

### 7.4 How Could I Forget: a `struct` is Passed by Value!

It's amazing how many things I ended up realizing towards the end of the project. 

Initially, all my sorting algorithms had their "sorter" structs passed to them by value. This was fine for most of the algorithms because they only had to copy the struct once (when they were calling the main routine of the algorithm). However, smooth sort had many subroutines, and each one was passed a *copy* of the struct. I somehow overlooked the problem, and for the last week or so I'd been using a heavily underoptimized version of smooth sort; it's even worse because the sorter struct for smooth sort was the largest out of all the algorithms... then, by mere serendipity, I was forced to refactor my sorting algorithms to measure their frequency counts.

While I was doing this, I had to update the function signatures to accept a pointer to the sorter struct instead of a copy of it; this would allow me to update the struct member holding the value of the curent frequency count for each algorithm. However, upon doing so, I noticed an abrupt change in the speed of smooth sort; it was actually able to compete with the other algorithms. Keep in mind that while I was doing the refactoring, I was also changing a number of other things, so the reason for this speed boost remained a mystery to me for a few minutes. Eventually, I realized what was going on and kept note of it here.

<br />

### 7.5 Correlation, NOT JUST Determination

Towards the latter half of the testing phase, I realized how much more valuable it would be to measure the *coefficient of correlation* and not just the coefficient of determination. While the initial idea was to use the latter metric (because I thought measuring 'shuffledness' was sufficient), I later realized that knowing the 'bias' of a dataset (whether it tends to be in the *right order* or in *reverse*) would just be as insightful, since some of the algorithms obviously perform differently based on this. In this case, correlation would definitely give us more insights to work with.

Due to the delay of this realization, I had to restart all the tests I had done at that point. All in all, counting mishaps and dry-runs, I probably left my laptop on for a total of at least 50 hours running tests on the algorithms. In case you're curious, my humble potato has an `Intel i3-6006u` processor, so it was definitely up for the task. It also uses Ubuntu, not Windows. 

<br />

### 7.6 My Smooth Sort Can Be Optimized Further

I'm saying this with a few hours left before the submission of the project... I don't have time to rerun all the tests with the optimization I wish to conduct. Nevertheless, I might decide to do this even after the project is over.

Basically, one of the existing subroutines within the current implementation was a left over from my initial draft of smooth sort. During the root sorting phase, my smooth sort used to need to compare the nodes of the trees with their children in order to insert the nodes correctly. This was because root sorting was carried out during the heapification phase, and so newly inserted roots would not necessarily belong to heapified trees at that moment. However, upon revising the algorithm a few days ago, I moved the root sorting phase to the latter half of the algorithm, so that roots were sorted during the actual sorting phase. This made it unnecessary to make the comparisons with the children of each root. Thus, if these extraneous comparisons are removed, there is a possibility for smooth sort to become much more efficient (although only by a small amount). 

On the other hand, we can place the root sorting subroutine back into the first half of the algorithm and do without the sorting phase completely, since heapifying in this manner would actually create the sorted array already. Whether or not this will be any faster, I do not know.

![author](./README/headers/header-author.png)
---

```
                                                    |\      _,,,---,,_
                                             ZZZzz /,`.-'`'    -.  ;-;;,_
                                                  |,4-  ) )-,_. ,\ (  `'-'
                                                 '---''(_/--'  `-'\_)
```
[neue-dev](https://github.com/neue-dev)

<br />

### 8.1 Author's Acknowledgements

> [1] *Correlation between two Decks of cards?* (n.d.). Cross Validated. https://stats.stackexchange.com/questions/78591/<br>
[2] Dijkstra, E. W. (1982). Smoothsort, an alternative for sorting in situ. *Science of Computer Programming, 1*(3), 223–233. https://doi.org/10.1016/0167-6423(82)90016-8<br>
[3] *Fast computing of log2 for 64-bit integers*. (n.d.). Stack Overflow. https://stackoverflow.com/questions/11376288/<br>
[4] GeeksforGeeks. (2023, February 22). *In-Place merge sort*. GeeksforGeeks. https://www.geeksforgeeks.org/in-place-merge-sort/<br>
[5] *How to generate a random int in C?* (n.d.). Stack Overflow. https://stackoverflow.com/questions/822323/<br>
[6] *John von Neumann’s remark on entropy*. (n.d.). MathOverflow. https://mathoverflow.net/questions/403036/<br>
[7] *Smoothsort demystified*. (n.d.). https://www.keithschwarz.com/smoothsort<br>
[8] Wikipedia contributors. (2024, April 3). *Fisher–Yates shuffle*. Wikipedia. https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle<br>
[9] Wikipedia contributors. (2024, April 27). *Heapsort*. Wikipedia. https://en.wikipedia.org/wiki/Heapsort<br>
[10] Wikipedia contributors. (2024, May 8). *Merge sort*. Wikipedia. https://en.wikipedia.org/wiki/Merge_sort<br>