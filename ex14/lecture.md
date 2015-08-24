Learn C The Hard Way
=======

Exercise 14
----

Writing and Using Functions




The Plan
====

* Write your very first functions.


The Code
====



The Analysis
====




Breaking It
====

* Remove the forward declarations to confuse the compiler and cause it to complains about *can_print_it* and *print_letters*.
* When you call *print_arguments* inside *main*, try
  adding 1 to *argc* so that it goes past the end of the
  *argv* array.



Extra Credit
====

* Rework these functions so that you have fewer functions.  For example,
  do you really need *can_print_it*?
* Have *print_arguments* figure out how long each argument string
  is by using the *strlen* function, and then pass that length
  to *print_letters*.  Then, rewrite *print_letters*
  so it only processes this fixed length and doesn't rely on the
  *'\0'* terminator.  You'll need the *#include <string.h>* for this.



Extra Credit
====

* Use *man* to look up information on *isalpha*
  and *isblank*.  Use other similar functions to
  print out only digits or other characters.
* Go read about how other people like to format their
  functions.  Never use the *K&R syntax* (it's antiquated and
  confusing) but understand what it's doing in case you run
  into someone who likes it.



End Of Lecture 14
=====


