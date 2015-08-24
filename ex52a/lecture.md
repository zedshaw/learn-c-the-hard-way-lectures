Learn C The Hard Way
=======

Exercise 52a
----

Hacking and Improving Your Server:

Project Description
----



The Plan
====

Learn to improve your server by hacking it.



The Purpose
====

Turn on defensive mode and destroy what you've created to make it better.



My Protocol Is Junk
====

The protocol should have specified sizes of elements,
or a correctly written parser using a parser generator.



Hacking The Protocol
====

* Fuzzing the names.
* Thrashing the size of strings.
* Putting '\0' in the data.
* Feeding crazy large numbers in.



The Rule
====

Protocols without exact grammars *or* fixed size elements will be hacked.



My Disk Storage Is Junk
====

"What?! C'mon!"



Hacking Your Disk
====

* Fuzzing the file names.
* Filling the disk.
* Rampant create/delete operations.
* Doing insanely long directories.
* Trying to access arbitrary files.



The Rule
====

The disk works, but you need a way to monitor storage or just have
tons of it.

Watch for arbitrary paths and *always* sanitize them.



First Code Review
====

I'll now do a "first pass" code review.

I just go through quick, looking for bugs and drop BUG: comments.




End of Lecture 52a
=====


