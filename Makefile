CFLAGS=-std=gnu89 -Wall -pedantic-errors -Os
all: my_bsearch_r my_readline my_strlcpy my_strtok_r
my_bsearch_r: my_bsearch_r.o
my_readline: my_readline.o
my_strlcpy: my_strlcpy.o
my_strtok_r: my_strtok_r.o
clean:; rm -f my_bsearch_r my_readline my_strlcpy my_strtok_r *.o
