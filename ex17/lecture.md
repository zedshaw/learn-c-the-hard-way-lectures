Learn C The Hard Way
=======

Exercise 17
----

Heap and Stack Memory Allocation



The Plan
====

* Learn to allocate data on the heap using *malloc*.
* Memory management techniques to avoid leaking.
* How the heap differs from the stack, and when to use them.



The Code
====



The Analysis
====




Breaking It
====

* The classic way is to remove some of the safety checks so that you can
  pass in arbitrary data. For example, remove the check on line 160
  that prevents you from passing in any record number.
* You can also try corrupting the data file.  Open it in any editor and
  change random bytes, and then close it.
* You could also find ways to pass bad arguments to the program when it's
  run. For example, getting the file and action backwards will make it create
  a file named after the action, and then do an action based on the first
  character.



Breaking It
====

* There's a bug in this program because *strncpy* is poorly
  designed.  Go read about *strncpy* and try to find out what happens
  when the *name* or *address* you give is *greater* than
  512 bytes.  Fix this by simply forcing the last character to *'\0'*
  so that it's always set no matter what (which is what strncpy should do).
* In the extra credit, I have you augment the program to create arbitrary
  size databases.  Try to see what the biggest database is before you
  cause the program to die due to lack of memory from *malloc*.



Extra Credit
====

* The *die* function needs to be augmented to let you pass the *conn*
  variable, so it can close it and clean up.
* Change the code to accept parameters for *MAX_DATA* and *MAX_ROWS*, store them in the *Database* struct, and write that to the file, thus creating
  a database that can be arbitrarily sized.
* Add more operations you can do with the database, like *find*.



Extra Credit
====

* Read about how C does it's struct packing, and then try to see why your
  file is the size it is.  See if you can calculate a new size after adding
  more fields.
* Add some more fields to *Address* and make them searchable.
* Write a shell script that will do your testing automatically for you
  by running commands in the right order.  Hint: Use *set -e* at the
  top of a *bash* to make it abort the whole script if any command
  has an error.



Extra Credit
====

* Try reworking the program to use a single global for the database connection.
  How does this new version of the program compare to the other one?
* Go research stack data structure and write one in your favorite language,
  then try to do it in C.



End Of Lecture 17
=====


