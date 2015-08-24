Learn C The Hard Way
=======

Exercise 19
----

Zed's Awesome Debug Macros



The Plan
====

* Learn about the macros that vastly improve my code quality.
* Find out why they help you out.
* Explore some advanced C Pre-Processor (CPP) macro magic code generation tricks.



The Code
====



The Analysis
====



Breaking It
====

These macros are designed on purpose to prevent you from doing this:

    if(blah) debug("This is a thing");
    else debug ("This is another thing");



Extra Credit
====

* Put ``#define NDEBUG`` at the top of the file and check that all
  of the debug messages go away.
* Undo that line, and add ``-DNDEBUG`` to ``CFLAGS`` at the
  top of the ``Makefile``, and then recompile to see the same thing.
* Modify the logging so that it includes the function name, as well
  as the ``file:line``.



End Of Lecture 19
=====


