Learn C The Hard Way
=======

Exercise 2
----

Using Makefiles to Build



The Plan
====

* Start with simple make usage.
* Set a few important settings.



How Make Works
====

Implied dependencies and ancient lore.



Shell Commands
====

    $ make ex1
    # or this one too
    $ CFLAGS="-Wall" make ex1
    $ make clean
    $ make ex1



Makefile
====

    CFLAGS=-Wall -g
    
    clean:
    	rm -f ex1



The Analysis
====

* Setting options.
* Indicating dependencies.
* Writing commends to run.


Breaking It
====

* Watch out for tabs vs. spaces.




Extra Credit
====

* Create an *all: ex1* target that will build *ex1* with
  just the command *make*.
* Read *man make* to find out more information on how to run it.
* Read *man cc* to find out more information on what the flags *-Wall* and *-g* do.
* Research *Makefiles* online and see if you can improve this one.
* Find a *Makefile* in another C project and try to understand
  what it's doing.



End Of Lecture 2
=====


