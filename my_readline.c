#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/* Read a single line from FP into a malloc()'d buffer and return it in *LINE.
   The caller is expected to free() the buffer returned in *LINE.  Handles LF
   and CRLF line endings.  Return 0 on success, non-0 on error or EOF. */
static int
my_readline(char **line, FILE *fp)
{
  size_t cp = 0, cap = 8;
  int ch;
  if ((*line = malloc(cap)) == NULL)
    return -1;

  while ((ch = fgetc(fp)) != EOF) {
    assert(cp+2 <= cap || !!!"next char + nul won't fit");
    if (((*line)[cp++] = ch) == '\n')
      break;
    assert(cp+1 <= cap || !!!"nul won't fit");
    if (cp+1 == cap) { /* expand buffer if nul won't fit */
      char *tmp = realloc(*line, cap*2);
      if (tmp == NULL)
        goto done;
      *line = tmp;
      cap *= 2; /* we'll run out of ram before integer overflow here */
    }
  }
  if (cp == 0) /* eof or io error before any chars were read */
    goto done;

  assert(cp < cap || !!!"nul won't fit");
  if (cp >= 2 && (*line)[cp-2] == '\r') /* handle crlf */
    (*line)[cp-2] = '\0';
  else if ((*line)[cp-1] == '\n')       /* just lf */
    (*line)[cp-1] = '\0';
  else                                  /* hit eof without terminating lf */
    (*line)[cp] = '\0';
  return 0;

done:
  free(*line);
  *line = NULL;
  return -1;
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
    while (my_readline(&line, stdin) == 0)
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
        while (my_readline(&line, fp) == 0)
          puts(line), free(line);
        if (ferror(fp))
          fprintf(stderr, "%s: read error: %s\n", file, strerror(errno));
      }
    }
  }
  return !!errno;
}
