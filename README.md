# 1. An Analysis of Sorting Algorithms
#### *by Mo David*

This repository currently only features a few select sorting algorithms. They are:

1. Insertion Sort
2. Selection Sort
3. Merge Sort
4. Heap Sort
5. Smooth Sort

In hindsight smooth sort was the most difficult to implement, but it also brought me most enjoyment. Because the implementations of the other algorithms are already widely-known, I put emphasis on explaining the mechanism of Smooth Sort below and leave out the others (except for Heap Sort... It'll become clear why I decided to explain this too).

Additionally, I'll outline the methods I used to benchmark the sorting algorithms. After much research, I decided to generate my own data sets (still using the `struct Record` provided by the starter code) to test the different implementations. I landed on a few ways to measure "entropy" (how shuffled an array is) to help me understand how the sorting algorithms would behave under different circumstances.

Likewise, to verify the amount by which the different algorithms would scale (in terms of execution time), the sizes of the custom data sets were varied too.

### 1.1 Running the Program

Compiling the program is relatively straightforward. Just use `gcc` and run `gcc main.c -o main`, after which you can run the built `main` executable. **For linux users**, I do leave another important note as the math library is not linked by default: if you are using a Linux device, run `gcc main.c -o main -lm` instead.

Depending on what exactly you want to run, the program accepts a number of different command-line parameters. They are listed below.

* 'toedit'
* 'addsmth'
* 'yeah'

So for example you could run the executable from above with the command `main yeah toedit`.

<!-- Detail how running the main program with different parameters can give different results -->

### 1.2 Doing Custom Tests

Because of the way the program is structured, it is possible to import `sortingalgorithms.c` into your own main function and test each of the following functions with your own data:

1. `Record_insertionSort(Record *records, int n)`
2. `Record_selectionSort(Record *records, int n)`
3. `Record_heapSort(Record *records, int n)`
4. `Record_mergeSort(Record *records, int n)`
5. `Record_smoothSort(Record *records, int n)`

Note how all the functions preserve the same signature to facilitate testing. Functions that use recursion (such as merge sort) have been abstracted into their own files to avoid succumbing to spaghetti code.

> <b style="color: rgba(255, 55, 55, 1); background-color: rgba(255, 55, 55, 0.16); padding: 4px 8px;">IMPORTANT REMINDERS</b>
>
> Please keep the following in mind to avoid any problems running your custom tests:
>
> 1. When importing `sortingalgorithms.c` and using any of the `Record_...Sort()` functions, <b style="color: rgba(255, 55, 55, 1);">DO NOT FORGET TO RUN</b> `Record_initSorters()` at the start of the main function (with no parameters). This function has to be run once before any of the sorting algorithms can work.
> 2. <b style="color: rgba(255, 55, 55, 1);">DO NOT MOVE</b> any of the files around to prevent breaking any of the imports.

# 2. Heap Sort

Heap sort has been described as ["selection sort using the right data structure"](https://link.springer.com/chapter/10.1007/978-3-030-54256-6_4). While that was not something that made sense to me the first time around, it was something that clicked eventually.

Heap sort treats the array differently: instead of viewing it as a sequential list of elements, heap sort visualizes it in the form of a tree, with the associations between parent nodes and child nodes outlined in a simple and defined manner. Every $i^\text{th}$ element in the array is the child of the $\lfloor \frac{n - 1}{2} \rfloor^\text{th}$ entry and a parent of elements $2i + 1$ and $2i + 2$. By defining the tree in this way, adjacent nodes can be found easily at the expense of just a multiplication or two.

### 2.1 Why Do We Use A Tree?

But what benefit does a tree have over an array of elements? Because of the structure of a heap, we are guaranteed to know that every element is greater than all its descendants. This invariant allows us to shift an element to its right place within the structure without having to compare it with every single element in the array.

### 2.2 The Heap Sort Algorithm

<!-- !To be continued -->

# 3. Smooth Sort

The reason I decided to explain heap sort prior to smooth sort is because the two algorithms rely on the same fundamental ideas: visualizing an array in a manner that neglects its linear structure. However, smooth sort attempts to remedy a certain problem with heap sort: the largest element of the array is always at the root (the beginning) of the array, when ultimately it must end up at the opposite end. This means that regardless of the initial state of our array, $n \cdot \log n$ operations must necessarily happen. Heap sort does not care whether or not our data is sorted to some degree.

Smooth sort, on the other hand, takes an unorthodox approach. For one thing, it doesn't create a single tree but rather a *forest of max-heaps*. For another, it builds these trees such that their root nodes are on the right. This entails way less computations for lists that are close to being sorted.

But how does it do this, exactly? That's where things get exciting.

### 3.1 Leonardo Numbers and Leonardo Heaps

### 3.2 Generating the Forest

### 3.3 The Smooth Sort Algorithm

<!-- !To be continued -->

# 4. Shuffling, Entropy, and Correlation

### 4.1. Shuffling

There is a well-known shuffling algorithm that generates a permutation of the elements of a sequence in a *reliably-random* way. By this, we mean to say that every possible permutation is equally likely, and the shuffling process does not favor any single one. This is called the [Fisher-Yates algorithm](https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle).

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

Entropy is often associated with the idea of disorder. Fundamentally, the concept of "disorder" is not too far from the concept of "messing up" a deck of cards, although context necessitates that we refer to the second case as [information entropy](https://en.wikipedia.org/wiki/Entropy_(information_theory)#:~:text=Generally%2C%20information%20entropy%20is%20the,referred%20to%20as%20Shannon%20entropy.). Interestingly enough, both information theory and statistical mechanics have characterized their notions of entropy in much the same way. We focus on the formulation Claude Shannon provided for information theory (who impressively came up with the form [independent of any knowledge of statistical mechanics](https://mathoverflow.net/questions/403036/john-von-neumanns-remark-on-entropy)):

<br>

$$
\begin{align}
E = -\sum_{x \in S} p_x \cdot ln(p_x)
\end{align}
$$

<br>

gives the entropy of a discrete random variable $X$, where for some $x$ in the sample space $S$, $p_x$ is $P(X=x)$. The important thing to see here is that we're multiplying the probabilities of each of the outcomes in the sample space by how "surprising" they are. That's all $-ln(p_x)$ is telling us, because:

<br>

$$
\begin{align}
& -ln(p_x) \\
& =ln(\frac{1}{p_x})
\end{align}
$$

<br>

When $p_x=1$, then the event isn't suprising at all and the value above reduces to $0$. On the other hand, for smaller values of $p_x$, the value above increases and indicates that the event is "more surprising". Vaguely speaking maximizing entropy means "maximizing surprise and uncertainty", and this is achieved by dispersing our random variable across all possible values of the sample space. If some event were highly likely, then most of the time the outcome wouldn't surprise us at all. But if every outcome were equally likely, then each outcome would be very "surprising" because we would have no idea which one would likely happen next (as a fair warning, don't take the word "surprising" to mean anything rigorous here; it's only meant to be an analogy).

So how can we use this to analyze the "shuffledness" of our arrays? Entropy manifests in our sequences of records when it becomes hard to expect which record comes after a given record. In a sorted list (or a list with some structure to it), this is easy because the records form a sort of pattern. Thus, when traversing the array, at any given point, it is reasonable to have an expectation for what the next record might be (it won't be anything surprising). However, when an array is shuffled really well, it becomes really hard to tell what record might come next, and so things are "uncertain" and "surprising". 

[To approach this more rigorously](https://stats.stackexchange.com/questions/78591/correlation-between-two-decks-of-cards/79552#79552), we define a random variable $X$ to represent the possible differences between any two adjacent records. That is, for any two adjacent records $r_i$ and $r_{i + 1}$ along the shuffled array, we define $x_i$ to be

<br>

$$
\begin{align}
x_i = s(r_{i + 1}) - s(r_{i})
\end{align}
$$

<br>

where $s(r_i)$ returns the index of the record *in the sorted array*. Thus, a highly entropic array will have $X$ vary considerably, whereas in a sorted array, $P(X=1)$ is $1$ and no other differences occur.

Do note that in order for this approach to work, we adjust negative differences to fall within the range $[0, N-1]$ ($N$ is the number of records). We do this by adding $N$ whenever $x_i < 0$. Effectively, this just gives us the residue of $x_i \text{ mod } N$. If we were to leave this step out, our computation for entropy would likely have its terms cancel out, and we would end up with 0 when in fact the system displays a high degree of entropy.

### 4.3. Correlation (or Rather, Determination)

[Another useful idea](https://stats.stackexchange.com/questions/78591/correlation-between-two-decks-of-cards/) to help us assess the "shuffledness" of an array is correlation. Using the same function $s(r_i)$ from above, we can create a rough estimate of the disorder in our array by determining the strength of the correlation between $i$ and $s(r_i)$. A sorted array will always have $i=s(r_i)$ (that is, the values of the sorted indices of the records are equal to their current position in the array), and would produce a correlation of exactly $1$. An unsorted array, however, will likely have a very small correlation value (something close to 0). Unlike entropy, our scale goes in the opposite direction within the interval $[0, 1]$.

However, there is something else we must account for: if the array was in reverse, it wouldn't be considered "well-shuffled" (it's far from sorted, but it's not really jumbled up) and correlation would give us a value of $-1$. This is fine because it isn't $0$ (which would indicate complete disorder), but it would be better to restrict all our values within the positive range of $[0, 1]$. To avoid dealing with negative numbers, we square the value we obtain for the correlation. This particular number is used a lot in inferential statistics and has a special name: [the coefficient of determination](https://en.wikipedia.org/wiki/Coefficient_of_determination). Although this number has a few different interpretations, we ignore most of these as they do not apply to our framework. However, we must acknowledge the relevance this metric has (henceforth referred to as $r^2$) in describing the disorder within a set of data.

# 5. Comparison and Individual Analyses

### 5.1 Comparisons

### 5.2 Analyses of Each Algorithm

### 5.3 Some Interesting Realizations

At some point I got kind of worried with my implementation of smooth sort because it seemed to behave in a manner that contradicted its $O(n\log n)$ promise. Since all three of our faster sorting algorithms (merge sort, heap sort, and smooth sort) must behave in this way (most of the time), I expected their execution times to scale by roughly the same amount for increasing dataset sizes. Indeed, when comparing heap sort and merge sort, this seemed to hold true. Assuming $P$ was held constant, when heap sort took twice as much time to sort a larger list, merge sort took about twice as much of its own time too.

However, when looking at smooth sort against those other two algorithms, it seemed to perform twice as much *and a little more*. The larger the datasets, the more this difference became apparent; it wasn't something you could just ignore. It seemed like there was another factor at play and for a moment I thought I introduced another factor to the runtime behaviour of my smooth sort. Did my implementation secretly have another multiplier, like $O(n\log n\cdot\log n)$? I tried in vain to verify the correctness of my implementation, but I was certain I wasn't performing any unnecessary steps.

And then it hit me. Whenever we say an algorithm runs in $O(\log n)$ (or in this case, $O(n \log n)$ ), we sometimes drop the base of the logarithm and leave it out when performing Big-O analyses. I got all these expressions for the different behaviours of our algorithms without actually paying mind to their underlying nuances. The moment we consider the difference of what $\log n$ means for all heap, merge and smooth sort, our comparisons between these algorithms begin to make sense.  

Heap and merge sort have the usual $\log_2 n$; the former uses a *binary* tree to perform sorting, while the latter uses a sequence of *divisions by 2*. Smooth sort, on the other hand, has $\log_\varphi$. For the uninitiated, 

<br>

$$
\begin{align}
\varphi &= \frac{\sqrt{5} + 1}{2}\\
\varphi &= 1.61803...
\end{align}
$$

<br>

and is often called the golden ratio. Logically, because smooth sort utilizes Leonardo numbers to define the structure of its heaps, the number of operations we perform must grow with respect to the number of Leonardo numbers under $n$. Since every Leonardo number [can be bounded by a multiple of a Fibonacci number](https://www.keithschwarz.com/smoothsort/), and every Fibonacci number [can be represented as a scaled power of the ratio above](https://en.wikipedia.org/wiki/Fibonacci_sequence#Closed-form_expression), then Leonardo numbers must grow by at least $\varphi^x$. In other words, given some $k \text{th}$ Leonardo number, we can be sure that $k$ is at least as great as $\alpha \log_\varphi n$ for some constant $\alpha$. The number of Leonardos under a given $n$ must thus be a logarithmic function (in base $\varphi$) of $n$.

After all this, we finally, we have the true behaviours of our sorting algorithms:

* Heap sort: $O(n \log_2 n)$
* Merge sort: $O(n \log_2 n)$
* Smooth sort: $O(n \log_\varphi n)$

Plotting these functions on desmos, we can see that smooth sort must scale differently from the other algorithms.

ALSO talkABOUT HOW WHEN COMPUTING THE RATIO OF THE FUNCTIONS, THE RATIO TENDS TO BE SMALL ( < 2 for the most part, and it grows logarithmically too>) BUT THIS IS ENOUGH TO CREATE BIG DIFFERENCES FOR LARGER DATA SETS

CONTINUE EXPLAINING, TALK ABOUT ADDING FACTOR alpha AND THE NEEDED CONSTANT $\frac{\sqrt{5}}{2}$ for smooth sort
<!-- ! insert graph of functions -->

# 6. Author

```
                                                    |\      _,,,---,,_
                                             ZZZzz /,`.-'`'    -.  ;-;;,_
                                                  |,4-  ) )-,_. ,\ (  `'-'
                                                 '---''(_/--'  `-'\_)
```