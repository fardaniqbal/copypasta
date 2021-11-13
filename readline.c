/* Public domain readline(): read a stream line-by-line without truncating
   input lines (which is not trivial with fgets()'s interface.)  NB: call
   interface differs from GNU's readline(). */
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#undef readline
#define readline my_readline_ /* avoid name collision with GNU readline */

/* Read a line from FP and return it in a malloc()'d buffer, handling LF
   and CRLF line endings.  Caller must free() the returned buffer.  Return
   NULL on EOF or error (i/o error or malloc failure).  Call feof(FP) to
   distinguish EOF vs. error. */
static char *
readline(FILE *fp)
{
  char *line, *tmp;
  size_t cp = 0, cap = 8;
  int ch;
  if ((line = malloc(cap)) == NULL) /* need at least 1 byte for nul */
    return NULL;

  while ((ch = fgetc(fp)) != EOF && ch != '\n') {
    assert(cp < cap || !!!"current char won't fit");
    line[cp++] = ch;
    if (cp < cap) /* next char/nul will fit, so no need to resize */
      continue;
    if (cap > ((size_t) -1)/2 || (tmp = realloc(line, cap *= 2)) == NULL)
      goto done; /* integer overflow on cap, or realloc failed */
    line = tmp;
  }
  if (ch == EOF && cp == 0) /* eof or io error before any chars were read */
    goto done;

  assert(cp < cap || !!!"nul won't fit");
  if (cp >= 1 && line[cp-1] == '\r') /* handle crlf */
    cp--;
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
    if (!feof(stdin))
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
        if (!feof(fp))
          fprintf(stderr, "%s: read error: %s\n", file, strerror(errno));
      }
    }
  }
  return !!errno;
}
