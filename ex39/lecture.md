Learn C The Hard Way
=======

Exercise 39
----

String Algorithms



The Plan
====

Develop a formal code review procedure.


The Code
====

The code is easy to implement so should be no problem for you
at this point.  Focus on getting the unit test right.



Code Review Process
====



1. Start at the entry point for a piece of code that has changed.
2. For each function, confirm that its calling parameters are correct.
3. Enter that function, and confirm each line's correctness.
4. When you encounter a function, repeat up to #2 until you go no further.
5. As you exit functions, confirm the return values and their usage.
6. Continue until you are back where you started at the entry point.
7. Do a diff on your changes, and confirm any missed calls to changed functions.



Code Review Key Points
====

1. Check your pointer dereferences and defend against NULL.
2. Check if-statements and while loops for exiting.
3. Check return values are going to be valid.
4. Check that memory allocated is freed and other resources freed.
5. Confirm all system call parameters are correct with man pages.



Record Your Code Review
====

I want *you* to try to record yourself coding and reviewing your
code.  What do you learn from this experience?

What if you kept track of the number of mistakes you found in
your code reviews and analyzed the data?



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


