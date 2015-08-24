Learn C The Hard Way
=======

Exercise 44
----

Ring Buffer



The Plan
====

Learn about a handy data structure for I/O processing:

Ring Buffers



The Code
====

It's basically a DArray with dynamic start and end settings.
You can *also* use a Queue of bstrings to do almost the same thing.



Code Review
====



The Analysis
====

* Watch a ring buffer work in the debugger.
* Draw it visually to explore it.
* The purpose is to efficiently add and remove data when the amount added and removed is random.



Pause!
====

I will next review the unit test I wrote so if you want to attempt
solving it yourself then pause now.



The Unit Test
====

Here's my version of the unit test.



Breaking It
====

* The biggest mistake you'll make with a ring buffer is off-by-one errors.
* This is why the RingBuffer\_commit\_ and other macros exist.
* Another common mistake is to use it between threads, but that's a whole other book.



Extra Credit
====

* Create an alternative implementation of ``RingBuffer`` that uses
  the POSIX trick and a unit test for it.
* Add a performance comparison test to this unit test that compares the
  two versions by fuzzing them with random data and random read/write operations.
  Make sure that you set up this fuzzing so that the same operations are done
  to each version, and you can compare them between runs.



End Of Lecture 44
=====


