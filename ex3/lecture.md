Learn C The Hard Way
=======

Exercise 3
----------

Formatted Printing




The Plan
====

* Introduction to *printf*.


The Code
====



The Analysis
====



Breaking It
====

* Take the *age* variable out of the first *printf* call, then recompile. You should get a couple of warnings.
* Run this new program and it will either crash or print out a really crazy age.
* Put the *printf* back the way it was, and then don't set *age* to an initial value by changing that line to *int age;*, and then rebuild it and run it again.




Extra Credit
====

* Find as many other ways to break *ex3.c* as you can.
* Run *man 3 printf* and read about the other *%* format
  characters you can use.  These should look familiar if you used
  them in other languages (they come from *printf*).
* Add *ex3* the *all* list in your *Makefile*.  Use this
  to *make clean all* and build all of your exercises thus far.
* Add *ex3* to your *clean* list in your*Makefile* as well.
  Use *make clean* to remove it when you need to.



End Of Lecture 3
=====


