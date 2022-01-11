This is a collection of miscellaneous code snippets that are intended to be
copied-and-pasted as needed.  Basically code that's too small to be worth the
hassle of turning into a library (even a [single file
library](https://github.com/nothings/single_file_libs).)  I've placed these in
the public domain, unless mentioned otherwise.

* bsearch_r.c - reentrant alternative to `bsearch`
* readline.c - read from files line-by-line.
* strlcpy.c - safer alternative to `strncpy`.
* strtok_r.c - reentrant alternative to `strtok`.
* xmalloc.c - wrappers around `malloc`/`realloc`/`calloc` that crash on
  allocation failure with a message indicating file/line of failure.
