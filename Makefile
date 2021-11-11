CFLAGS=-std=gnu89 -Wall -pedantic-errors -Os -g3
all: bsearch_r qsort_r readline strlcpy strtok_r xmalloc
bsearch_r: bsearch_r.o
qsort_r: qsort_r.o
readline: readline.o
strlcpy: strlcpy.o
qsort_r: qsort_r.o
xmalloc: xmalloc.o
clean:; rm -f bsearch_r qsort_r readline strlcpy strtok_r xmalloc *.o
