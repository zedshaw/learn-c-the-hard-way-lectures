Learn C The Hard Way
=======

Exercise 48
----

A Simple Network Server:

Project Description
----



The Plan
====

Start your first long running project:

statserve



The Purpose
====

You'll get the project started and get a minimum first hack going.



The Requirements
====

1. Create a simple network server that accepts a connection on port 7899 from
   *netclient* or the *nc* command, and echoes back anything you type.
2. You'll need to learn how to bind a port, listen on the socket, and answer it.
   Use your research skills to study how this is done and attempt to implement it
   yourself.



The Requirements
====

3. The more important part of this project is laying out the project directory
   from the *c-skeleton*, and making sure you can build everything and get it
   working.
4. Don't worry about things like daemons or anything else.  Your server just has
   to run from the command line and keep running.



The Clues
====

I will now give you some clues:

* USE liblcthw!
* Remember you did a client already, you just need to make a server.
* Do NOT use select! Use fork() for the server.
* Keep it *simple*.  Don't worry about anything other than accepting a connection and closing.
* Stay small, build slowly.



Important References
====

* Research online for "echo server in C".
* Read man (2) pages for *accept*, *bind*, *listen*, *connect*, *select*, *socket*, and *shutdown*.



Encouragement
====

This will be *HARD*!  Try it your best, and take it piece by piece.  You can do it, but remember if you give up the next video (48b) will show you the code to my solution and how to solve it.  You can peek there then come back when you're stuck.



End Of Lecture 48a
=====


