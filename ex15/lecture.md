Learn C The Hard Way
=======

Exercise 15
----

Pointers, Dreaded Pointers



The Plan
====

* A long video on C pointers.
* Lots of demonstration and visuals.



The Code
====



The Pointer Lexicon
====

    type *ptr A pointer of type named ptr
    *ptr The value of whatever ptr is pointed at
    *(ptr + i) The value of (whatever ptr is pointed at plus i)
    &thing The address of thing
    type *ptr = &thing A pointer of type named ptr set to the address of thing
    ptr++ Increment where ptr points



Pointers Visually
====



The Analysis
====




Breaking It
====

* Try to make *cur_age* point at *names*.  You'll need to
  use a C cast to force it, so go look that up and try to figure it out.
* In the final *for-loop*, try getting the math wrong in weird ways.
* Try rewriting the loops so that they start at the end of the arrays and go
  to the beginning.  This is harder than it looks.



Extra Credit
====

* Rewrite all of the arrays in this program as pointers.
* Rewrite all of the pointers as arrays.
* Go back to some of the other programs that use arrays and
  try to use pointers instead.
* Process command line arguments using just pointers similar to how
  you did *names* in this one.
* Play with combinations of getting the value of and the address of
  things.
* Add another *for-loop* at the end that prints out the
  addresses that these pointers are using.  You'll need the *%p* format
  for *printf*.



Extra Credit
====

* Rewrite this program to use a function for each of the ways you're
  printing out things.  Try to pass pointers to these functions so that
  they work on the data.  Remember you can declare a function to accept
  a pointer, but just use it like an array.
* Change the *for-loops* to *while-loops* and see what
  works better for which kind of pointer usage.



End Of Lecture 15
=====


