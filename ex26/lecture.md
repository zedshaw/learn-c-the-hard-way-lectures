Learn C The Hard Way
=======

Exercise 26
----

Project logfind



The Plan
====

Attempt your first project!

logfind
----



How Projects Work
====

The projects in this book are designed to make you apply
what you know so far to something "real world".

1. I will tell you when to *pause* so you can try to solve it yourself.
2. You will be given the challenge.  Pause!
3. You will be given clues. Pause!
4. Finally the solution.
5. Then I try to break my own solution.



The Code
====

If you ever get super stuck, you can visit:

https://github.com/zedshaw/learn-c-the-hard-way-lectures

To get all of the code for this book.



The Challenge
====

I want a tool called ``logfind`` that let's me search through log files for
text.  This tool is a specialized version of another tool called ``grep``, but
designed only for log files on a system.



The Challenge
====

* This tool takes any sequence of words and assumes I mean "and" for them.  So ``logfind zedshaw smart guy`` will find all files that have ``zedshaw`` *and* ``smart`` *and* ``guy`` in them.
* It takes an optional argument of ``-o`` if the parameters are meant to be *or* logic.
* It loads the list of allowed log files from ``~/.logfind``.



The Challenge
====

* The list of file names can be anything that the ``glob`` function allows.  Refer to ``man 3 glob`` to see how this works.  I suggest starting with just a flat list of exact files, and then add ``glob`` functionality.
* You should output the matching lines as you scan, and try to match them as fast as possible.



Demo
====

Here is a demo of me using the one I wrote.



Pause!
====

Now it's time for you to attempt to solve it from just this idea.



The Clues
====

* Remember to solve it a piece at a time.
* Start with just getting the arguments.
* Then figure out how to open files and just open the ones in this directory.
* Then figure out how to read the files.
* Then find out how to find the arguments in the files.
* Then figure out how glob works.
* Then use glob to find the files and open them.

It helps to do each of these in *main()* then "carve" them out into their
own functions.



Pause!
====



The Solution
====



Breaking It
====



End Of Lecture 26
=====


