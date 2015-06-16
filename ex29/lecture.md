Learn C The Hard Way
=======

Exercise 29
----

Libraries and Linking



The Plan
====

* Learn about libraries and how to link against them.
* Learn how to load a dynamic library from inside C.



The Code
====

I'll use the project I started from the previous exercise.
This covers some of the extra credit.



The Analysis
====

This analysis might take a while, but be sure you know Exercise 28 well.



Breaking It
====

* Wreck the libex29.so file.



Extra Credit
====

* Were you paying attention to the bad code I have in the ``libex29.c`` functions?
  Do you see how, even though I use a for-loop they still check for ``'\0'``
  endings?  Fix this so that the functions always take a length for the
  string to work with inside the function.
* Read the ``man dlopen`` documentation and read about all of the
  related functions.  Try some of the other options to ``dlopen``
  beside ``RTLD_NOW``.




End Of Lecture 29
=====


