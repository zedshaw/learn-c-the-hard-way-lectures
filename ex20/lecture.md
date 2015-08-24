Learn C The Hard Way
=======

Exercise 20
----

Advanced Debugging Techniques



The Plan
====

Demonstrate more advanced debugging techniques and tools.


The Demonstration
====




Extra Credit
====

* Find a graphical debugger and compare using it to raw ``gdb``.
  These are useful when the program you're looking at is local, but they
  are pointless if you have to debug a program on a server.
* You can enable core dumps on your OS, and when a program crashes,
  you'll get a core file.  This core file is like a postmortem of
  the program that you can load up to see what happened right at the crash
  and what caused it.  Change ``ex31.c`` so that it crashes
  after a few iterations, then try to get a core dump and analyze it.



End Of Lecture 20
=====


