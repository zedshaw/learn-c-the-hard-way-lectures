Learn C The Hard Way
=======

Exercise 38
----

Hashmap Algorithms



The Plan
====

Learn three different string hashing algorithms and make them dynamically available
to the Hashmap.



Code Review
====

The default is the Jenkin's hash.

You added the FNV1a, Adler32, and DJB hashing algorithms.

Review the code for FNV1a vs. DJB.



Breaking It
====

In this exercise you will attempt to write the worst hashing function that can
pass for a real one.  Try to make one that either looks complicated but
statistically is way off, or is a discrete change to an existing one that is a
bad change.



Extra Credit
====

* Take the ``default_hash`` out of the ``hashmap.c``, make it
  one of the algorithms in ``hashmap_algos.c``, and then make all
  of the tests work again.
* Add the ``default_hash`` to the ``hashmap_algos_tests.c``
  test and compare its statistics to the other hash functions.
* Find a few more hash functions and add them, too. You can never have too
  many hash functions!



End Of Lecture 38
=====


