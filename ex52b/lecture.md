Learn C The Hard Way
=======

Exercise 52b
----

Hacking and Improving Your Server

Solution
----



The Plan
====

A more formal code review to demonstrate how I think 
about my own code.



The Purpose
====

Teach you that you *never* assume your code is good. 

You're a scientist not a god. 

Always assume there's a chance you're wrong.



Code Review Of Protocol Handling
====

A quick code review of the protocol handling then simple hacking.




Hacking Demo
====

This is an ultra light hacking demo.

I could do much worse if I had more time.



Fixing These Flaws
====

* Add rate limiting calculations with stat.h
* Add limits on name lengths.
* Change to a md5 or sha1 hash, the encipher thing is dumb.
* Forking kind of saves us ironically, but it has to go.

Look at the github repository to see all of the things I fixed.




End of Lecture 52b
=====


