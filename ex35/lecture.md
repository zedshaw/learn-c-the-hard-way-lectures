Learn C The Hard Way
=======

Exercise 35
----

Sorting and Searching



The Plan
====

* Make a simple DArray sorting library using existing functions.
* Implement a new structure and algorithm called a "Radix Map".
* Create a binary search algorithm for the RadixMap.



The DArray Code
====

Continuing the code review method with a part of DArray.



The RadixMap Code
====

Code review this code next.



The Binary Search Code
====

Finally, code review of the BSTree code.



Improving It
====

* Use a binary search to find the minimum position for the
  new element, then only sort from there to the end.  You find the
  minimum, put the new element on the end, and then just sort from
  the minimum on.   This will cut your sort space down
  considerably most of the time.
* Keep track of the biggest key currently being used, and then only
  sort enough digits to handle that key.  You can also keep track
  of the smallest number, and then only sort the digits necessary
  for the range.  To do this, you'll have to start caring about
  CPU integer ordering (endianness).



Extra Credit
====

* Implement quicksort, heapsort, and merge sort and then provide a *#define*
  that lets you pick between the two, or create a second set of functions
  you can call.  Use the technique I taught you to read the Wikipedia page
  for the algorithm, and then implement it with the psuedo-code.
* Compare the performance of your optimizations to the original implementations.



Extra Credit
====

* Use these sorting functions to create a *DArray_sort_add* that
  adds elements to the *DArray* but sorts the array after.
* Write a *DArray_find* that uses the binary search algorithm from
  *RadixMap_find* and the *DArray_compare* to find elements
  in a sorted *DArray*.




End Of Lecture 35
=====


