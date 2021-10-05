/* Public domain implementation of POSIX function strtok_r() for non-POSIX
   systems.  Example of typical usage:

       char input[] = "parse this string into whitespace-delimited tokens";
       char *tok, *ctx, *str = input;
       for (; (tok = strtok_r(str, " \t\r\n", &ctx)) != NULL; str = NULL)
         process_token(tok);

   See POSIX docs for more info. */
#include <string.h>
#undef strtok_r
#define strtok_r my_strtok_r_ /* prevent name collision on POSIX systems */

/* Parse STR into a sequence of DELIM-delimited tokens.  The first call returns
   the first token from STR.  Call with STR = NULL to get subsequent tokens.
   Return NULL when no more tokens remain.  *CONTEXT stores the parser's state
   between successive calls.  This function writes to STR, so behavior is
   undefined if STR is constant (e.g., "literals like this"). */
static char *
strtok_r(char *str, const char *delim, char **context)
{
  char *tok, *s = str ? str : *context;
  for (*context = s; *s != '\0' && strchr(delim, *s); s++)
    continue;
  for (tok = s; *s != '\0' && !strchr(delim, *s); *context = ++s)
    continue;
  if (*s != '\0')
    *context = s + 1;
  *s = '\0';
  return *tok == '\0' ? NULL : tok;
}

/* - test ------------------------------------------------------------------ */

#undef NDEBUG
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/* Verify that strtok_r() tokenizes STR as expected.  Specify the expected
   tokens in the var args.  The final var arg must be NULL. */
static void
verify_strtok(const char *str, const char *delim, ...)
{
  char *buf, *bp, *tok, *ctx;
  va_list ap;
  if ((buf = malloc(strlen(str) + 1)) == NULL)
    fprintf(stderr, "malloc() failed\n"), exit(1);
  strcpy(buf, str);
  va_start(ap, delim);
  for (bp = buf; (tok = strtok_r(bp, delim, &ctx)) != NULL; bp = NULL) {
    /* TODO */
  }
  va_end(ap);
  free(buf);
}

int
main(void)
{
  /* TODO */
  (void) verify_strtok;
  return 0;
}
