Learn C The Hard Way
=======

Exercise 49
----

A Statistics Server:

Solution
----



The Plan
====

I'll show you how I implemented the protocol in the smallest code possible.

I won't implement all of the CRUD operations, so you can go look at the 
git repo for this project to see a full implementation.



The Setup
====

First I setup the data, then the protocol parser, then the handlers.



The Protocol
====

    create Create a new statistic.
    mean   Get the current mean of a statistic.
    sample Add a new sample to a statistics.
    dump   Get all of the elements of a statistic (sum, sumsq, n, min, and max).Final Code



The Command Structure
====

    typedef struct Command {
        bstring command;
        bstring name;
        bstring number;
        handler_cb handler;
    } Command;



The Storage Record
====

    typedef struct Record {
        bstring name;
        Stats *stat;
    } Record;



The Design
====

* Accept a connection
* Parse the line into the Command
* Run a handler function to process it
* Temporarily store into a Hashmap



Final Thoughts
====

The last thing I would do is add better tests and round out the protocol with CRUD operations.



End Of Lecture 49b
=====


