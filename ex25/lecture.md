Learn C The Hard Way
=======

Exercise 25
----

Variable Argument Functions



The Plan
====

* Use variable argument functions.
* Write our own simple version of *scanf*.



The Code
====



The Analysis
====



Breaking It
====

* Change the code so that you forget to pass in the initial size for '%s' formats.
* Give it more data than ``MAX_DATA``, and then see how omitting ``calloc`` in ``read_string`` changes how it works.
* There's a problem where fgets eats the newlines, so try to fix that using
``fgetc`` but leave out the ``\0`` that ends the string.



Extra Credit
====

* Make double and triple sure that you know what each of the ``out_``
  variables are doing.  Most importantly, you should know what is ``out_string`` is and how it's
  a pointer to a pointer, , so that you understand when you're setting the pointer versus the
  contents is important.  Break down each of the



Extra Credit
====

* Write a similar function to ``printf`` that uses the varargs system,
  and rewrite ``main`` to use it.
* As usual, read the man page on all of this so that you know what it does
  on your platform.  Some platforms will use macros, others will use
  functions, and some will have these do nothing.  It all depends on the
  compiler and the platform you use.



End Of Lecture 25
=====

