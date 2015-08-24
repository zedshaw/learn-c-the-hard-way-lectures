Learn C The Hard Way
=======

Exercise 37
----

Hashmaps



The Plan
====

Implement a Hashmap in C.
In Python these are called Dictionaries.


Hashmaps Visually
====

Hashmaps are very intuitive once you know how a DArray works.
It's all about the hashing function used.



Code Review
====

Conducting a review of Hashmap by following the test.



Improving It
====

* You can use a sort on each bucket so that they're always sorted.
  This increases your insert time but decreases your find time, because
  you can then use a binary search to find each node.  Right now,
  it's looping through all of the nodes in a bucket just to find one.
* You can dynamically size the number of buckets, or let the caller
  specify the number for each *Hashmap* created.
* You can use a better *default_hash*.  There are tons of them.



Improving It
====

* This (and nearly every *Hashmap*) is vulnerable to someone picking
  keys that will fill only one bucket, and then tricking your program
  into processing them.  This then makes your program run slower because
  it changes from processing a *Hashmap* to effectively processing
  a single *DArray*.  If you sort the nodes in the bucket, this
  helps, but you can also use better hashing functions, and for
  the really paranoid programmer, add a random salt so that keys can't be predicted.



Improving It
====

* You could have it delete buckets that are empty of nodes to save space,
  or put empty buckets into a cache so you can save on time lost creating and destroying
  them.
* Right now, it just adds elements even if they already exist.  Write an
  alternative set method that only adds an element if it isn't set already.



Extra Credit
====

* Research the *Hashmap* implementation in your favorite programming language to see what features it has.
* Find out what the major disadvantages of a *Hashmap* are and how to avoid them.  For example, it doesn't preserve order without special changes, nor does it work when you need to find things based on parts
  of keys.
* Write a unit test that demonstrates the defect of filling a *Hashmap* with keys that land
  in the same bucket, then test how this impacts performance.  A good way to do this is to just reduce
  the number of buckets to something stupid, like five.



End Of Lecture 37
=====


