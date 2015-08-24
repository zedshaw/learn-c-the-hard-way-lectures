Learn C The Hard Way
=======

Exercise 47
----

A Fast URL Router



The Plan
====

Use the *TSTree* to do something useful:

Route URLs



Code Review
====



The Analysis
====

Watch me play with it and then tell you how it's working.



Improving It
====

* Collect all possible matches then choose the longest as winner.
* Use TSTree to find prefixes, then regex to choose winner.



Extra Credit
====

* Instead of just storing the string for the handler, create an actual engine that uses a
  *Handler* struct to store the application.  The structure would store the URL to which it's attached, the name, and anything else you'd need to make an actual routing system.



Extra Credit
====

* Instead of mapping URLs to arbitrary names, map them to .so files and use the *dlopen*
  system to load handlers on the fly and call callbacks they contain.  Put these callbacks that
  in your *Handler* struct, and then you have yourself a fully dynamic callback
  handler system in C.



End Of Lecture 47
=====


