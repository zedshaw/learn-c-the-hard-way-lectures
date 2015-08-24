Learn C The Hard Way
=======

Exercise 50
----

Routing the Statistics

Solution
----



The Plan
====

Show you how I solved the problem of routing the names of statistics as URLs.



The Setup
====

1. First thing I did was make sure my tests were really good.
2. Then I designed the data structures I'd need.
3. Then I did the work to make them functions.
4. The protocol shouldn't need to change.



"URLs"
====

I'll define paths as simply names separated by /.

Real URLs are way more complex than that.



Data Structure
====

I just added:

    struct bstrList *path;

To the Command struct to hold paths.



URL Meaning
====

Kind of weird, but:

    Deepest part of URL is "parent", this is the main stat.
    Children are next segments up, and are mean-of-mean stats.



New Processing
====

1. Change to a loop over all paths with a "scan path" function.
2. Add optional path parameter to handlers.
3. Parse the path in *parse\_command* to set path in Commands.
4. In sample and create, change root processing vs. child processing.
5. Move *send\_reply* over to *net.c* instead.



Test First Path Parsing
====

I'll write a small test for just the *scan\_paths* part first.

Then wire that in and use the existing tests to confirm the old code
works.



The Code
====



Final Review
====



End Of Lecture 50b
=====


