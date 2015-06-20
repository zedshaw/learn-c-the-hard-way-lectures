Learn C The Hard Way
=======

Exercise 11
----

Arrays and Strings




The Plan
====

* Learn the similarity between arrays and strings.
* Avoid getting pedantic about them.
* Learn how C stores strings and processes them.


The Code
====



The Analysis
====




Breaking It
====

So many ways to break this!

* Get rid of the initializers that set up *name*.
* Accidentally set *name[3] = 'A';* so that there's no terminator.
* Set the initializer to *{'a','a','a','a'}* so that there are too many
  'a' characters and no space for the *'\0'* terminator.



Extra Credit
====

* Assign the characters into *numbers*, and then use *printf*
  to print them one character at a time.  What kind of compiler warnings
  do you get?
* Do the inverse for *name*, trying to treat it like an array
  of *int* and print it out one *int* at a time.  What
  does the debugger think of that?
* In how many other ways can you print this out?



Extra Credit
=====

* If an array of characters is 4 bytes long, and an integer is 4 bytes
  long, then can you treat the whole *name* array like it's just
  an integer?  How might you accomplish this crazy hack?
* Take out a piece of paper and draw each of these arrays as a
  row of boxes. Then do the operations you just did on paper to see
  if you get them right.
* Convert *name* to be in the style of *another* and see
  if the code keeps working.



End Of Lecture 11
=====


