CFLAGS=-std=gnu89 -Wall -pedantic-errors -Os
my_readline: my_readline.o
clean:; rm -f my_readline *.o
