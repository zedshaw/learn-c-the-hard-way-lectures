Learn C The Hard Way
=======

Exercise 34
----

Dynamic Array



The Plan
====

Learn about dynamic arrays, a very useful datastructure.



Code Review
====

Starting with the header file to implement, then the test, then the implementation.



The Analysis
====



DArray Advantages
====

* Iteration:  You can just use a basic for-loop and ``DArray_count``
  with ``DArray_get``, and you're done.  No special macros needed, and
  it's faster because you aren't walking through pointers.
* Indexing:  You can use ``DArray_get`` and ``DArray_set`` to
  access any element at random, but with a ``List`` you have to go
  through N elements to get to N+1.
* Destroying:  You can just free the struct and the ``contents`` in
  two operations.  A ``List`` requires a series of ``free`` calls
  and walking every element.



DArray Advantages
=====

* Cloning: You can also clone it in just two operations (plus whatever
  it's storing) by copying the struct and ``contents``.  A list
  again requires walking through the whole thing and copying every ``ListNode``
  plus its value.
* Sorting: As you saw, ``List`` is horrible if you need to keep the
  data sorted.  A ``DArray`` opens up a whole class of great sorting
  algorithms, because now you can access elements randomly.
* Large Data: If you need to keep around a lot of data, then a ``DArray``
  wins since its base, ``contents``, takes up less memory than the same
  number of ``ListNode`` structs.



DArray Disadvantages
====

* Insert and remove on the front (what I called shift).  A ``DArray``
  needs special treatment to be able to do this efficiently, and usually it
  has to do some copying.
* Splitting or joining:  A ``List`` can just copy some pointers and
  it's done, but with a ``DArray``, you have copy all of the
  arrays involved.



DArray Disadvantages
====

* Small Data. If you only need to store a few elements, then typically the
  storage will be less in a ``List`` than a generic ``DArray``. This is because
  the ``DArray`` needs to expand the backing store to accommodate future
  inserts, while a ``List`` only makes what it needs.



Breaking It
====

* Forget to check the return value from malloc and then use the buffer.
* Getting the end and start count of the buffer wrong.  Easy to do an off-by-one here.
* Exploit the insert and delete costs to cause a denial of service.



Extra Credit
====

* Improve the unit tests to cover more of the operations, and test them
  using a for-loop to ensure that they work.
* Research what it would take to implement bubble sort and merge sort
  for DArray, but don't do it yet.  I'll be implementing DArray algorithms
  next, so you'll do this then.



Extra Credit
====

* Write some performance tests for common operations and compare them
  to the same operations in ``List``.  You did some of this already, but this
  time, write a unit test that repeatedly does the operation in question, and
  then in the main runner, do the timing.



Extra Credit
====

* Look at how the ``DArray_expand`` is implemented using a constant increase (size + 300).
  Typically, dynamic arrays are implemented with a multiplicative increase (size * 2), but I've
  found this to cost needless memory for no real performance gain.  Test my assertion
  and see when you'd want a multiplicative increase instead of a constant increase.




End Of Lecture 34
=====


