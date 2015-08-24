Learn C The Hard Way
=======

Exercise 32
----

Double Linked Lists



The Plan
====

Learn about your very first data structure:

Double Linked Lists
----



Creating A liblcthw Project
====

We'll need a project for the rest of the book called *liblcthw*.



Algorithms and Data Structures
====

A big step in going from amateur to professional is learning
about data structures and algorithms.

A double linked list is the easiest one.



Double Linked Lists Visually
====

I'll quickly draw some diagrams to show you how they work.



Automated Testing Demo
====

You can enter the code just fine, but watch me write
the test.



Code Reviews
====

Later videos will demonstrate how to code review to make code solid.



Improving It
====

* You can make ``List_clear_destroy`` more efficient by using
  ``LIST_FOREACH`` and doing both ``free`` calls inside one
  loop.
* You can add asserts for preconditions so that the program isn't given a ``NULL``
  value for the ``List *list`` parameters.



Improving It
====

* You can add invariants that check that the list's contents are always correct,
  such as ``count`` is never ``< 0``, and if ``count > 0``, then ``first`` isn't NULL.
* You can add documentation to the header file in the form of comments before
  each struct, function, and macro that describes what it does.



Extra Credit
====

* Research doubly vs. singly linked lists and when one is preferred over
  the other.
* Research the limitations of a doubly linked list.  For example, while they
  are efficient for inserting and deleting elements, they are very slow for
  iterating over them all.
* What operations are missing that you can imagine needing?  Some examples
  are copying, joining, and splitting.  Implement these operations and write the
  unit tests for them.



End Of Lecture 32
=====


