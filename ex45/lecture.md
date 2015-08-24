Learn C The Hard Way
=======

Exercise 45
----

A Simple TCP/IP Client



The Plan
====

* Learn to use the *select* method and a RingBuffer to write a simple command line network client.


How select Works
====



Code Review
====



Improving It
====

These read functions are useful so I can put them in RingBuffer.



Extra Credit
====

* As I mentioned, there are quite a few functions you may not know, so
  look them up.  In fact, look them all up even if you think you know
  them.
* Go back through and add various defensive programming checks to
  the functions to improve them.
* Use the *getopt* function to allow the user
  the option *not* to translate *\n* to *\r\n*. This
  is only needed on protocols that require it for line endings, like HTTP.
  Sometimes you don't want the translation, so give the user the option.



End Of Lecture 45
=====


