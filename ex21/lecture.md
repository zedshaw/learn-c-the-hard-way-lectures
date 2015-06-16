Learn C The Hard Way
=======

Exercise 21
----

Advanced Data Types and Flow Control



The Plan
====

* Learn about the basic types and keywords for them.
* Cover all the keywords for modifying those types.
* Review fixed exact size types.
* Learn all the different operators on those types.

This is mostly a review!



Available Data Types
====

    int    Stores a regular integer, defaulting to 32 bits in size.
    double Holds a large floating point number.
    float  Holds a smaller floating point number.
    char   Holds a single 1 byte character.
    void   Indicates "no type".
    enum   Enumerated types, which work as and convert to integers.



Type Modifiers
====

    unsigned  Non-negative numbers.
    signed    Gives you negative and positive numbers.
    long      Bigger number.
    short     Smaller number.



Type Qualifiers
====

    const     Constant.
    volatile  Compiler can't trust it.
    register  Put it in a CPU register.



Type Conversion
====

C type promotion order:

* long double
* double
* float
* int (but only char and short int);
* long

When in doubt, parens it out!



Exact Size Types
====

If you need exact sizes use these:

    int8_t   8-bit signed integer
    uint8_t  8-bit unsigned integer
    int16_t  16-bit signed integer
    uint16_t 16-bit unsigned integer
    int32_t  32-bit signed integer
    uint32_t 32-bit unsigned integer
    int64_t  64-bit signed integer
    uint64_t 64-bit unsigned integer



Getting Sizes
====

Refer to the book as there's a large number of
macros to help you get size information for types.

Examples:

    int_least32_t  int that holds at least 32 bits.
    uint_fast32_t  unsigned fastest int for 32 bits.
    intptr_t       signed int that can hold a pointer.
    PTRDIFF_MAX    maximum value of ptrdiff_t
    SIZE_MAX       maximum value of a size_t



Available Operators
-------------------

This section is a review of what you memorized already
to make sure you know everything.

Memorize these again to be sure you have them.



Extra Credit
====

* Read stdint.h or a description of it, and write out all the
  available size identifiers.
* Go through each item here and write out what it does in code.  Research it online so you know you got it right.
* Get this information memorized by making flash cards and spending 15
  minutes a day practicing it.
* Create a program that prints out examples of each type, and confirm that your
  research is right.



End Of Lecture 21
=====


