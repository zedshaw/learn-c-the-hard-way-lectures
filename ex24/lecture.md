Learn C The Hard Way
=======

Exercise 24
----

Input, Output, Files



The Plan
====

* Learn the basics of working with files in C.
* Get an initial list of the "f-functions".



The Code
====



The Analysis
====



Breaking It
====

* Trying out *fgets* and the problems with *gets*.
* Feed it */dev/urandom* to give it garbage.



Extra Credit
====

* Rewrite this to not use ``fscanf`` at all.  You'll need to use
  functions like ``atoi`` to convert the input strings to numbers.
* Change this to use plain ``scanf`` instead of ``fscanf`` to
  see what the difference is.
* Fix it so that their input names get stripped of the trailing newline
  characters and any whites pace.



Extra Credit
====

* Use ``scanf`` to write a function that reads one character at a time
  and files in the names but doesn't go past the end.  Make this function
  generic so it can take a size for the string, but just make sure you end
  the string with ``'\0'`` no matter what.



End Of Lecture 24
=====


