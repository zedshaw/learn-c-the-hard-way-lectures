Learn C The Hard Way
=======

Exercise 36
----

Safer Strings



The Plan
====

Learn about an alternative string implementation to avoid most C string problems.



C Strings Suck
====

It is impossible to safely process strings in C.



The bstrling Library
====

An alternative is a library that provides alternative APIs for working with
C strings.



The Common Functions
====

    bfromcstr    Create a bstring from a C style constant.
    blk2bstr     Do the same thing, but give the length of the buffer.
    bstrcpy      Copy a bstring.
    bassign      Set one bstring to another.
    bassigncstr  Set a bstring to a C string's contents.
    bassignblk   Set a bstring to a C string but give the length.
    bdestroy     Destroy a bstring.
    bconcat      Concatenate one bstring onto another.
    bstricmp     Compare two bstrings returning the same result as strcmp.
    biseq        Tests if two bstrings are equal.



The Common Functions
====

    binstr       Tells if one bstring is in another.
    bfindreplace Find one bstring in another, then replace it with a third.
    bsplit       Split a bstring into a bstrList.
    bformat      Do a format string, which is super handy.
    blength      Get the length of a bstring.
    bdata        Get the data from a bstring.
    bchar        Get a char from a bstring.



Extra Credit
====

There is only one extra credit and that's to write a *tests/bstr_tests.c* file that
tests all of these functions.  The bstrlib comes with a test that you can reference it if needed.



End Of Lecture 36
=====


