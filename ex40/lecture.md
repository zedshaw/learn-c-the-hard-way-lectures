Learn C The Hard Way
=======

Exercise 40
----

Binary Search Trees



The Plan
====

Implement a Binary Search Tree, a competitor to the Hashmap.



Binary Search Trees Visually
====



The Code
====

There's nothing new in the code, but make sure you read the book carefully.



Code Review
====

I'll walk through the implementation and compare it to Hashmaps for features.



Improving It
====

* As usual, you should go through all of the defensive programming checks and add
  *assert*s for conditions that shouldn't happen.  For example, you shouldn't be getting *NULL* values for the recursion functions, so assert that.
* The traverse function walks through the tree in order by traversing left, then right,
  and then the current node.  You can create traverse functions for the reverse order, as well.



Improving It
====

* It does a full string compare on every node, but I could use the *Hashmap*
  hashing functions to speed this up.  I could hash the keys, and then keep the hash in
  the *BSTreeNode*.  Then, in each of the set up functions, I can hash the
  key ahead of time, and pass it down to the recursive function.  Using this hash, I can
  then compare each node much quicker in a way that's similar to what I do in *Hashmap*.



Breaking It
====

A big flaw in this is the use of recursion.  An attacker could choose data to cause a stack overflow.



Extra Credit
====

* There's an alternative way to do this data structure without using recursion.  The Wikipedia
  page shows alternatives that don't use recursion but do the same thing.  Why would this
  be better or worse?
* Read up on all of the different but similar trees you can find. There are AVL trees (named after Georgy Adelson-Velsky and E.M. Landis), red-black trees,
  and some non-tree structures like skip lists.



End Of Lecture 40
=====


