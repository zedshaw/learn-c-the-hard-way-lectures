Learn C The Hard Way
=======

Exercise 46
----

Ternary Search Tree



The Plan
====

Learn about my favorite data structure ever:

Ternary Search Tree



The Code
====

Similar to a Binary Search Tree, but it has 3 branches per node based on
the characters in strings.



Advantages
====

* Find any string comparing at most N characters.
* Detect *missing* strings as fast, usually faster.
* Find all strings that start with, or contain, any substring as fast.
* Find all similar known strings quickly.



Disadvantages
====

* Delete is a pain, as in most trees.
* Uses lots of memory to store keys, so bad for sets of large keys.
* Kind of weird for most programmers.



Improving It
===

* You could allow duplicates by using a *DArray* instead of the
  *value*.
* As I mentioned earlier, deleting is hard, but you could simulate it by setting
  the values to *NULL* so that they are effectively gone.
* There are no ways to collect all of the possible matching values.  I'll have
  you implement that in an extra credit.
* There are other algorithms that are more complex but have slightly
  better properties.  Take a look at suffix array, suffix tree, and
  radix tree structures.



Extra Credit
====

* Implement a *TSTree_collect* that returns a *DArray* containing
  all of the keys that match the given prefix.
* Implement *TSTree_search_suffix* and a *TSTree_insert_suffix*
  so you can do suffix searches and inserts.
* Use the debugger to see how this structure is used in memory 
  compared to the *BSTree* and *Hashmap*.



End Of Lecture 46
=====


