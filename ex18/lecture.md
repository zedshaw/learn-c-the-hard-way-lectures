Learn C The Hard Way
=======

Exercise 18
----

Pointers to Functions



The Plan
====

* Advanced topic of pointers to functions.
* These are very useful but not encountered too often.



The Code
====



The Analysis
====




Breaking It
====

Let's hack your computer with this code:

    unsigned char *data = (unsigned char *)cmp;
    
    for(i = 0; i < 25; i++) {
        printf("%02x:", data[i]);
    }

    printf("\n");

You'll see how the bytes of code that make up your program can also be data.


Extra Credit
====

* Get a hex editor and open up *ex18*, and then find the sequence
  of hex digits that start a function to see if you can find the function
  in the raw program.
* Find other random things in your hex editor and change them.  Rerun your
  program and see what happens.  Strings you find are the easiest
  things to change.
* Pass in the wrong function for the *compare_cb* and see what
  the C compiler complains about.
* Pass in NULL and watch your program seriously bite it.  Then, run
  the debugger and see what that reports.
* Write another sorting algorithm, then change *test_sorting* so
  that it takes *both* an arbitrary sort function and the sort function's
  callback comparison.  Use it to test both of your algorithms.



End Of Lecture 18
=====


