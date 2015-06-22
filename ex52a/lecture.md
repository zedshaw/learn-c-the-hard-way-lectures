Learn C The Hard Way
=======

Exercise 52
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



Your Protocol Is Junk
====

"What?! But you told me to write it!"



Hacking The Protocol
====

* Fuzzing the names.
* Thrashing the size of strings.
* Putting '\0' in the data.
* Feeding crazy large numbers in.



The Rule
====

Protocols without exact grammars *or* fixed size elements will be hacked.



Your Disk Storage Is Junk
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



Other Ways To Break It
====

Review your own code using our defensive techniques and see if you can find more holes.  How many can you find?



End Of Lecture 52a
=====


