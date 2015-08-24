Learn C The Hard Way
=======

Exercise 16
----

Structs And Pointers To Them



The Plan
====

* Learn to work with *structs* to structure data and make new types.
* Learn to use pointers to work with *structs* better.



The Code
====



The Analysis
====




Breaking It
====

* Try passing *NULL* to *Person_destroy* see what
  it does.  If it doesn't abort, then you must not have the
  *-g* option in your Makefile's *CFLAGS*.
* Forget to call *Person_destroy* at the end, and then run
  it under the debugger to see it report that you forgot
  to free the memory.  Figure out the options you need to pass
  to the debugger to get it to print how you leaked
  this memory.



Breaking It
====

* Forget to free *who->name* in *Person_destroy*
  and compare the output.  Again, use the right options to
  see how the debugger tells you exactly where you messed
  up.
* This time, pass *NULL* to *Person_print* and
  see what the debugger thinks of that. You'll figure out that *NULL* is a quick way
  to crash your program.



Extra Credit
====

* How to create a *struct* on the *stack* just like you're making any other variable.
* How to initialize it using the *x.y* (period) character
  instead of the *x->y* syntax.
* How to pass a structure to other functions without using
  a pointer.



End Of Lecture 16
=====


