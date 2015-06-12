Learn C The Hard Way
=======

Exercise 10
----

Switch Statements



The Plan
====

* Learn about the *switch-statement* and indirectly jump tables.
* Write a program that takes a command line argument.



The Code
====



The Analysis
====

Let's talk about jump tables, in the naive sense.




Breaking It
====

* Forget a *break*, and it'll run two or more blocks of code you don't want it to run.
* Forget a *default*, and it'll silently ignore values you forgot.
* Accidentally put a variable into the *switch* that evaluates to something unexpected, like an *int*, which becomes weird values.
* Use uninitialized values in the *switch*.




Extra Credit
====

* Write another program that uses math on the letter to
  convert it to lowercase, and then remove all of the extraneous
  uppercase letters in the switch.
* Use the *','* (comma) to initialize *letter*
  in the *for-loop*.
* Make it handle all of the arguments you pass it with
  yet another *for-loop*.



Extra Credit
====

* Convert this *switch-statement* to an *if-statement*.
  Which do you like better?
* In the case for 'Y' I have the break outside of the *if-statement*. What's the impact of this,
  and what happens if you move it inside of the *if-statement*. Prove to yourself that you're right.




End Of Lecture 10
=====


