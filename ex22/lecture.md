Learn C The Hard Way
=======

Exercise 22
----

The Stack, Scope, and Globals



The Plan
====

* Start to learn about scope.
* Stack vs. global.
* Scope levels inside a function.
* The *extern* keyword.


The Code
====

This exercises requires two files:

    * ex22.c
    * ex22_main.c



The Analysis
====



Fixing It
====

Instead of breaking this one I'm going to fix it.

* Do not shadow a variable like *count* on ex22_main.c:11.
* Avoid using too many globals.
* When in doubt, put it on the heap (malloc).
* Don't use function static variables like I did in ex22.c:update_ratio.
* Avoid reusing function parameters.



Breaking It
====

* Try to directly access variables in ``ex22.c`` from ``ex22_main.c``
  that you think you can't.  For example, can you get at ``ratio``
  inside ``update_ratio``? What if you had a pointer to it?
* Ditch the ``extern`` declaration in ``ex22.h`` to see what
  errors or warnings you get.
* Add ``static`` or ``const`` specifiers to different variables,
  and then try to change them.



Extra Credit
====

* Research the concept of pass by value verses pass by reference.  Write an
  example of both.
* Use pointers to gain access to things you shouldn't have access to.
* Use your debugger to see what this kind of access looks like when you
  do it wrong.
* Write a recursive function that causes a stack overflow.  Don't know
  what a recursive function is?  Try calling ``scope_demo`` at the
  bottom of ``scope_demo`` itself so that it loops.
* Rewrite the ``Makefile`` so that it can build this.



End Of Lecture 22
=====


