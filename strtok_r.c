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

/* Tokenize STR into DELIM-delimited tokens.  A call with non-NULL STR returns
   the first token.  Follow-up calls with STR = NULL return subsequent tokens.
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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/* verify(EXPR, FMT, ...) crashes with printf-like message FMT if !EXPR. */
#define verify (verify_file = __FILE__, verify_line = __LINE__, verify_)
static const char *verify_file;
static int verify_line;

#if defined(__GNUC__) || defined(__llvm__)
__attribute__((format(printf, 2, 3)))
#endif
static void
verify_(int expr, const char *fmt, ...)
{
  va_list ap;
  if (expr)
    return;
  fprintf(stderr, "%s:%d: ", verify_file, verify_line);
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  fflush(NULL);
  va_end(ap);
  exit(1);
}

#define STRTOK_R_DEBUG 0

static void
verify_strtok_asdf(int call_depth, const char *str, const char *delim, ...)
{
  const char *expect;
  va_list ap;
  fprintf(stderr, "%*s%s verify_strtok_asdf: depth=%d, str='%s'\n",
          call_depth, "", "entering", call_depth, str);
  va_start(ap, delim);
  while ((expect = va_arg(ap, char *)) != NULL)
    fprintf(stderr, "%*s  expected='%s'\n", call_depth, "", expect);
  va_end(ap);
  fprintf(stderr, "%*s%s verify_strtok_asdf\n", call_depth, "", "exiting");
}

/* Verify that strtok_r() tokenizes STR as expected.  Specify the expected
   tokens in the var args.  The final var arg must be NULL. */
static void
verify_strtok(int call_depth, const char *str, const char *delim, ...)
{
  char *buf, *bp, *tok, *ctx;
  va_list ap;
#if STRTOK_R_DEBUG
  {
    const char *expect;
    fprintf(stderr, "%*s entering verify_strtok(depth=%d, str='%s')...\n",
            call_depth, "", call_depth, str);
    fprintf(stderr, "%*s depth %d, str='%s'\n", call_depth, "", call_depth, str);
    va_start(ap, delim);
    while ((expect = va_arg(ap, char *)) != NULL)
      fprintf(stderr, "%*s addr=%16p, ", call_depth, "", expect),
        fprintf(stderr, "expected token = '%s'\n", expect);
    va_end(ap);
  }
#endif
  if (call_depth >= 3) /* yes, call depth limit is indeed arbitrary */
    return;
  if ((buf = malloc(strlen(str) + 1)) == NULL)
    fprintf(stderr, "malloc() failed\n"), exit(1);
  strcpy(buf, str);
  va_start(ap, delim);
  for (bp = buf; (tok = strtok_r(bp, delim, &ctx)) != NULL; bp = NULL) {
    const char *expect = va_arg(ap, char *);
#if STRTOK_R_DEBUG
    fprintf(stderr, "%*s  expect='%s'\n", call_depth, "", expect);
    fprintf(stderr, "%*s  tok = '%s'\n", call_depth, "", tok);
#endif
    verify(expect != NULL, "extra token '%s' in '%s'", tok, str);
    verify(!strcmp(tok, expect), "bad token '%s'; expected '%s'", tok, expect);
    verify(*tok != '\0', "strtok_r returned empty token from '%s'", str);
    /*verify_strtok(call_depth+1, "verify strtok_r maintains reentrancy", " ",
                  "verify", "strtok_r", "maintains", "reentrancy", 0);*/
  }
#if STRTOK_R_DEBUG
  {
    const char *expect = va_arg(ap, char *);
    fprintf(stderr, "%*s exiting verify_strtok(depth=%d, str='%s')\n",
            call_depth, "", call_depth, str);
    verify(expect == NULL, "didn't get all tokens in '%s' ('%p')", str, expect);
  }
#else
  verify(va_arg(ap, char *) == NULL, "didn't get all tokens in '%s'", str);
#endif
  va_end(ap);
  free(buf);
}

#if STRTOK_R_DEBUG
# define do_the_thing(args) \
    (verify_strtok_asdf args, verify_strtok args, fprintf(stderr, "\n\n----\n\n"))
#else
# define do_the_thing(args) verify_strtok args
typedef int verify_unused[!!sizeof &verify_strtok_asdf];
#endif

int
main(void)
{
  /* NOTE: C++ often defines NULL as 0 (instead of, e.g., ((void*)0)).  Since 0
     is of type int and not pointer (which may be passed to variadic functions
     differently, esp. on 64-bit architectures), passing NULL as a var arg
     that's expected to be a pointer is _incorrect_ when compiling as C++.
     What's worse: this undefined behavior may be masked depending on compiler
     optimization flags.  Therefore, we define a pointer-typed variable NP set
     to NULL, and pass NP to variadic functions instead of NULL. */
  const void *const NP = NULL;
  do_the_thing((0, "this is a test", " ", "this", "is", "a", "test", NULL));
  do_the_thing((0, "1 2 3", " ", "1", "2", "3", NULL));
  verify_strtok(0, "multiple  consecutive    delimiters", " ",
                "multiple", "consecutive", "delimiters", NULL);
  do_the_thing((0, " \r\nleading.delimiters", " \r\n.",
                "leading", "delimiters", NULL));
  do_the_thing((0, "trailing|-|delimiters-|/", "|-/",
                "trailing", "delimiters", NULL));
  do_the_thing((0, "", "", NULL)); /* empty string, no delims */
  do_the_thing((0, "", "delimiters", NULL)); /* empty string */
  do_the_thing((0, "this should be a single token", "", /* no delims */
                "this should be a single token", NULL));
  do_the_thing((0, "don't,return,,,empty,tokens,,,", ",",
                "don't", "return", "empty", "tokens", NULL));
  do_the_thing((0, "one-token", " ", "one-token", NULL));
  do_the_thing((0, "one-token", "", "one-token", NULL));
  return 0;
}
