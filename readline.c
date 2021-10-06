/* Public domain readline() implementation: read a stream line-by-line.  Unlike
   fgets(), this is _not_ limited to fixed-size output string, and does not
   truncate input lines.  NB: different call interface vs. GNU's readline(). */
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#undef readline
#define readline my_readline_ /* avoid name collision with GNU readline */

/* Read a line from FP and return it in a malloc()'d buffer, handling LF and
   CRLF line endings.  Caller must free() the returned buffer.  Return NULL on
   EOF or error.  Caller can distinguish EOF vs. i/o error with ferror(FP), and
   other errors with errno (malloc() failure, etc). */
static char *
readline(FILE *fp)
{
  char *line, *tmp;
  size_t cp = 0, cap = 8;
  int ch;
  if ((line = malloc(cap)) == NULL)
    return NULL;

  while ((ch = fgetc(fp)) != EOF) {
    assert(cp+2 <= cap || !!!"next char + nul won't fit");
    if ((line[cp++] = ch) == '\n')
      break;
    assert(cp+1 <= cap || !!!"nul won't fit");
    if (cp+1 < cap) /* next char + nul will fit, so don't need to resize */
      continue;
    cap *= 2; /* we'll run out of ram before integer overflow here */
    if ((tmp = realloc(line, cap)) == NULL)
      goto done;
    line = tmp;
  }
  if (cp == 0) /* eof or io error before any chars were read */
    goto done;

  assert(cp < cap || !!!"nul won't fit");
  if (cp >= 2 && line[cp-2] == '\r') /* handle crlf */
    line[cp-2] = '\0';
  else if (line[cp-1] == '\n')       /* just lf */
    line[cp-1] = '\0';
  else                               /* hit eof without terminating lf */
    line[cp] = '\0';
  return (tmp = realloc(line, cp+1)) ? tmp : line; /* trim if possible */

done:
  free(line);
  return NULL;
}

/* - test ------------------------------------------------------------------ */

#include <errno.h>
#include <string.h>

/* Read each given file line-by-line, and print each line to stdout.  Read from
   stdin if no file is given on the command line. */
int
main(int argc, char *argv[])
{
  char *line;
  if (argc == 1) {
    while ((line = readline(stdin)) != NULL)
      puts(line), free(line);
    if (ferror(stdin))
      fprintf(stderr, "read error: %s\n", strerror(errno));
  } else {
    while (--argc) {
      char *file = *++argv;
      FILE *fp;
      errno = 0;
      if ((fp = fopen(file, "r")) == NULL)
        fprintf(stderr, "%s: fopen() failed: %s\n", file, strerror(errno));
      else {
        while ((line = readline(fp)) != NULL)
          puts(line), free(line);
        if (ferror(fp))
          fprintf(stderr, "%s: read error: %s\n", file, strerror(errno));
      }
    }
  }
  return !!errno;
}
