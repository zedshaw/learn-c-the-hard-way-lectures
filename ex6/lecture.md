Learn C The Hard Way
=======

Exercise 6
----

Memorizing C Syntax



The Plan
====

* Memorize the keywords of C.
* Memorize the major syntax forms.



Execution Keywords
====
    break     Exit out of a compound statement.
    case      A branch in a switch-statement.
    continue  Continue to the top of a loop.
    do        Start a do-while loop.
    default   Default branch in a switch-statement.
    else      An else branch of an if-statement.
    for       Start a for-loop.
    goto      Jump to a label.
    if        Starts an if-statement.
    return    Return from a function.
    switch    Start a switch-statement.
    while     Start a while-loop.



Type Keywords
=====
    char      Character data type.
    double    A double floating point data type.
    float     A floating point data type.
    int       An integer data type.
    long      A long integer data type.
    short     A short integer data type.
    void      Declare a data type empty.
    union     Start a union-statement.
    struct    Combine variables into a single record.



Data Keywords
=====

    auto      Give a local variable a local lifetime.
    const     Make a variable unmodifiable.
    enum      Define a set of int constants.
    extern    Declare an identifier is defined externally.
    register  Declare a variable be stored in a CPU register.
    signed    A signed modifier for integer data types.
    sizeof    Determine the size of data.
    static    Preserve variable value after its scope exits.
    typedef   Create a new type.
    unsigned  An unsigned modifier for integer data types.
    volatile  Declare a variable might be modified elsewhere.



If-Statement
=====

    if(TEST) {
        CODE;
    } else if(TEST) {
        CODE;
    } else {
        CODE;
    }



Switch-Statement
=====

    switch (OPERAND) {
        case CONSTANT:
            CODE;
            break;
        default:
            CODE;
    }



While-Loop
==========

    while(TEST) {
        CODE;
    }



While with Continue
====

    while(TEST) {
        if(OTHER_TEST) {
            continue;
        }
        CODE;
    }



While with Break
=====

    while(TEST) {
        if(OTHER_TEST) {
            break;
        }
        CODE;
    }



Do-While
=====

    do {
        CODE;
    } while(TEST);



For-Loop
=====

    for(INIT; TEST; POST) {
        CODE;
    }

* *continue* and *break* work with *for*



Enum
=====

    enum { CONST1, CONST2, CONST3 } NAME;



Goto
====

    if(ERROR_TEST) {
        goto fail;
    }

    fail:
        CODE;



Functions
====

    TYPE NAME(ARG1, ARG2, ..) {
        CODE;
        return VALUE;
    }



Typedef
====

    typedef DEFINITION IDENTIFIER;


    typedef unsigned char byte;



Struct
====

    struct NAME {
        ELEMENTS;
    } [VARIABLE_NAME];



Typedef Struct
====

    typedef struct [STRUCT_NAME] {
        ELEMENTS;
    } IDENTIFIER;



Union
====

    union NAME {
        ELEMENTS;
    } [VARIABLE_NAME];



End Of Lecture 6
=====


