Learn C The Hard Way
=======

Exercise 23
----

Meet Duff's Device



The Plan
====

Learn the most evil awesome hack ever:

Duff's Device



The Code
====

Remember that this is *bad* code.
It's very interesting though, so struggle with it.



The Analysis
====

Before you continue, try to figure out what this does.
Consider it a debugging problem.



Clues
====

* Print this code out so that you can write on some paper.
* Write each of the variables in a table as they
  look when they get initialized right before the ``switch-statement``.
* Follow the logic to the switch, then do the jump to the right case.
* Update the variables, including the ``to``, ``from``, and the
  arrays they point at.



Clues
====

* When you get to the ``while`` part or my ``goto`` alternative,
  check your variables, and then follow the logic either back to the
  top of the ``do-while`` or to where the ``again`` label is
  located.
* Follow through this manual tracing, updating the variables, until
  you're sure you see how this flows.



Pause!
=====

I will then show you the solution so pause if you do
*NOT* want to see it yet.



Solving It
====

Watch me walk through how this works to see if it matches what you did.



Extra Credit
====

* Never use this again.
* Go look at the Wikipedia entry for Duff's device and see if you can
  spot the error.  Read the article, compare it to the version I have here, and try to understand why the Wikipedia code won't work for you
  but worked for Tom Duff.
* Create a set of macros that lets you create any length of device like this.
  For example, what if you wanted to have 32 case statements and didn't want
  to write out all of them? Can you do a macro that lays down eight at a time?



Extra Credit
====

* Change the ``main`` to conduct some speed tests to see which one is
  really the fastest.
* Read about ``memcpy``, ``memmove``, and ``memset``, and also compare
  their speed.
* Never use this again!



End Of Lecture 23
=====


