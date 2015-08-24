Learn C The Hard Way
=======

Exercise 49
----

A Statistics Server

Project Description
----



The Plan
====

Make the *statsserver* do something using a simple protocol.



The Purpose
====

Learn the first steps in creating a server that answers a protocol.



The Requirements
====

Create this protocol:

    create Create a new statistic.
    mean   Get the current mean of a statistic.
    sample Add a new sample to a statistics.
    dump   Get all of the elements of a statistic (sum, sumsq, n, min, and max).



The Requirements
====

1. You'll need to allow people to name these statistics, which means using one of the map style data structures to map names to ``Stats`` structs.
2. You'll need to add the ``CRUD`` standard operations for each name.  CRUD stands for create read update delete.  Currently, the list of commands above has create, mean, and dump for reading; and sample for updating.  You need a delete command now.
3. Make the protocol *strict*! Abort any client that makes any mistakes in protocols.



Strict Protocol
====

Once again, in case you missed it, be ruthless!

Abort all deviant clients.


Pause!
====

I'm going to give you clues to solve this, so if you want to try on your own pause now!



The Clues
====

* Create the data structures first for holding the information for each of these commands.
* Then write a protocol parser to handle it and fill in the data.
* Then pass that data to a function that knows how to do that command.
* You can just store the stats in a Hashmap, BSTree, or TSTree for now.
* KEEP IT SIMPLE!



Important References
====

* You'll want to refer to the bstring documentation as much as possible to know what functions to use.



Encouragement
====

* Remember that this is *supposed* to be hard.
* You are *supposed* to struggle with this.
* This could take you a while, but keep up the struggle, do it bit by bit, and test little pieces as you go.
* Automate your tests!



End Of Lecture 49a
=====


