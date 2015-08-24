Learn C The Hard Way
=======

Exercise 43
----

A Simple Statistics Engine



The Plan
====

* A fun and handy little statistics engine for simple analysis.
* Comparing it to the same in R.



Comparing Test vs. R
====

I'll use R to show you how this works vs. normal calculations using all data.



Breaking It
====

Easiest way to break this is to just feed it bad data once then the whole
stream is broken.



Extra Credit
====

* Convert the ``Stats_stddev`` and ``Stats_mean`` to ``static inline`` functions in the ``stats.h`` file instead of in the ``stats.c`` file.
* Use this code to write a performance test of the ``string_algos_test.c``.
  Make it optional, and have it run the base test as a series of samples, and then report
  the results.
* Write a version of this in another programming language you know.  Confirm that this
  version is correct based on what I have here.



Extra Credit
====

* Write a little program that can take a file full of numbers and spit these statistics
  out for them.
* Make the program accept a table of data that has headers on one line, then all
  of the other numbers on lines after it are separated by any number of spaces.  Your program
  should then print out these statistics for each column by the header name.



End Of Lecture 43
=====


