Learn C The Hard Way
=======

Exercise 51
----

Storing the Statistics

Solution
----



The Plan
====

Show you how I solved the problem of storing the statistics to disk.




Security Requirements
====

* Use *realpath* to make sure that paths are in one place.
* Use _BAD_ encryption to mangle the stored names.
* No other security beyond that. Just a demo of the path issue.



XTEA Encryption
====

* For an extra challenge I decided to "hash" names with XTEA.
* https://en.wikipedia.org/wiki/XTEA for the code.
* Normally I wouldn't do this, but wanted to show you XTEA.
* Because XTEA if cool and fun, although broken.
* DON'T USE XTEA FOR ENCRYPTION.



Improvements
====

* Let commands set cmd->path = NULL to indicate non-recursive.
* Change *echo_server* to *run_server* finally.
* Allow a 3rd storage path argument on the command line.
* Allow an additional argument to Command.



Weirdness
====

* Forking means I can't share data between clients without storage.
* Storing doesn't happen automatically, only explicitly.
* Loading acts as a copy command.
* XTEA isn't the best algorithm at all.  Just for fun.



How I Did It
====

1. Create the LOAD and STORE handlers.
2. Add Command.arg and set those in *parse\_command*.
3. Move path parsing up to allow non-recursive handling with path = NULL.
4. Write a *sanitize\_location* and *encrypt\_armor\_name* function, test them.
5. Write *handle\_store* first to allow testing *handle\_load*.
6. Use *open* (man 2 open) with O_EXLOCK to get exclusive locks on files.
7. Using *close* (man 2 close) should release EXLOCK, but not clear on this.



The Code
====



Final Review
====



End Of Lecture 51b
=====


