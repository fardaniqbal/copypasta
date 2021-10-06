CFLAGS=-std=gnu89 -Wall -pedantic-errors -Os
all: bsearch_r readline strlcpy strtok_r
bsearch_r: bsearch_r.o
readline: readline.o
strlcpy: strlcpy.o
strtok_r: strtok_r.o
clean:; rm -f bsearch_r readline strlcpy strtok_r *.o
