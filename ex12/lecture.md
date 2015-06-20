Learn C The Hard Way
=======

Exercise 12
----

Sizes and Arrays




The Plan
====

* Learn about *sizeof* and how it relates to arrays.



The Code
====



The Analysis
====




Breaking It
====

* Get rid of the *'\0'* at the end of *full_name*
  and re-run it.  Run it under the debugger, too.  Now, move the definition
  of *full_name* to the top of *main* before *areas*.
  Try running it under the debugger a few times and see if you get some
  new errors.  In some cases, you might still get lucky and not catch
  any errors.
* Change it so that instead of *areas[0]* you try to
  print *areas[10]*.  See what the debugger thinks of that.
* Try other ways to break it like this, doing it to *name* and
  *full_name* too.




Extra Credit
====

* Try assigning to elements in the *areas* array with *areas[0] = 100;* and similar.
* Try assigning to elements of *name* and *full_name*.
* Try setting one element of *areas* to a character from *name*.
* Search online for the different sizes used for integers on different
  CPUs.




End Of Lecture 12
=====


