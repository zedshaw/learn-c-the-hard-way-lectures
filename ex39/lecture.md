Learn C The Hard Way
=======

Exercise 39
----

String Algorithms



The Plan
====

Implement the Boyer-Moore-Horspool string search algorithm.
Use statistics to see if it beats the one in *bstrlib*.



The Code
====

The code is easy to implement so should be no problem for you
at this point.  Focus on getting the unit test right.



The Analysis
====

We will now use the data from the unit test to see if BMH is faster than
binstr.  Could have surprising results on a different computer.



Breaking It
====

In this exercise I want you to break the analysis rather than find out
how to break the code.  How can you get the stats wrong?  I'll show you.



Extra Credit
====

* See if you can make the ``Scan_find`` faster.  Why is my implementation
  here slow?
* Try some different scan times and see if you get different numbers.
  What impact does the length of time that you run the test have on
  the ``scan`` times?  What can you say about that result?
* Alter the unit test so that it runs each function for a short burst
  in the beginning to clear out any warm up period, and then start the
  timing portion.  Does that change the dependence on the length of time
  the test runs?  Does it change how many operations per second are possible?



Extra Credit
====

* Make the unit test randomize the strings to find and then measure
  the performance you get.  One way to do this is to use the ``bsplit``
  function from ``bstrlib.h`` to split the ``IN_STR`` on
  spaces.  Then, you can use the ``bstrList`` struct that you get to access each
  string it returns.  This will also teach you how to use ``bstrList``
  operations for string processing.
* Try some runs with the tests in different orders to see if you get different
  results.



End Of Lecture 39
=====


