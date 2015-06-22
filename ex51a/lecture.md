Learn C The Hard Way
=======

Exercise 51
----

Storing the Statistics

Project Description
----



The Plan
====

Learn to store the statistics to the hard disk.

There are meany issues with this.



The Purpose
====

To teach you about various problems related to securely storing files.



The Requirements
====

For this exercise, you'll add two commands for storing to and loading statistics
from a hard drive:

store
    If there's a URL, store it to a hard drive.

load
    If there are two URLs, load the statistic from the hard drive based on the first URL, and then put it into the second URL that's in memory.



The Requirements
====

1. If URLs have ``/`` characters in them, then that conflicts with the filesystem's use of slashes.  How will you solve this?
2. If URLs have ``/`` characters in them, then someone can use your server to overwrite files on a hard drive by giving paths to them.  How will you solve this?
3. If you choose to use deeply nested directories, then traversing directories to find files will be very slow.  What will you do here?



The Requirements
====

4. If you choose to use one directory and hash URLs (oops, I gave a hint), then directories with too many files in them are slow.  How will you solve this?
5. What happens when someone loads a statistic from a hard drive into a URL that already exists?
6. How will someone running ``statserve`` know where the storage should be?



The Clues
====

There are no clues.  You can do this.



End Of Lecture 51a
=====


