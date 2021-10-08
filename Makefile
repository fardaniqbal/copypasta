CFLAGS=-std=gnu89 -Wall -pedantic-errors -Os -g3
all: bsearch_r readline strlcpy strtok_r xmalloc
bsearch_r: bsearch_r.o
readline: readline.o
strlcpy: strlcpy.o
strtok_r: strtok_r.o
xmalloc: xmalloc.o
clean:; rm -f bsearch_r readline strlcpy strtok_r xmalloc *.o
