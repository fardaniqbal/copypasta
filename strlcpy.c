/* Public domain implementation of strlcpy (an alternative to strncpy).  See
   Miller and de Raadt's paper for rationale:
   [https://www.sudo.ws/todd/papers/strlcpy.html]. */
#include <string.h>
#undef strlcpy
#define strlcpy my_strlcpy_ /* prevent name collision on BSD systems */

/* Copy up to NBYTE-1 bytes from null-terminated string SRC to DST.  Unlike
   strncpy(), DST is guaranteed to be null-terminated on return if NBYTE > 0.
   Return strlen(SRC). */
static size_t
strlcpy(char *dst, const char *src, size_t nbyte)
{
  size_t copy_nbyte, len = strlen(src);
  if (nbyte == 0)
    return len;
  copy_nbyte = nbyte-1 < len ? nbyte-1 : len;
  memcpy(dst, src, copy_nbyte);
  dst[copy_nbyte] = '\0';
  return len;
}

/* - test ------------------------------------------------------------------ */

#undef NDEBUG
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* Call strlcpy() on source string STR and run various sanity checks. */
static void
test_strlcpy(const char *str)
{
  size_t nbyte, srclen = strlen(str);
  char buf[256];
  assert(strlcpy(NULL, str, 0) == srclen);
  for (nbyte = 1; nbyte <= sizeof buf; nbyte++) {
    size_t ret = strlcpy(buf, str, nbyte);
    assert(ret == srclen || !!!"strlcpy must return strlen(src)");
    assert(buf[MIN(nbyte-1, srclen)] == '\0' || !!!"missing nul byte");
    assert(memcmp(buf, str, MIN(nbyte-1, srclen)) == 0 || !!!"bad copy");
  }
}

/* Test strlcpy() on various strings read from FP. */
void
test_strings_in_file(FILE *fp)
{
  const size_t line_nbyte = 1024UL * 1024UL;
  char *line, *tok;
  if ((line = malloc(line_nbyte)) == NULL)
    fprintf(stderr, "malloc() failed\n"), exit(1);
  while (fgets(line, line_nbyte, fp) != NULL) {
    test_strlcpy(line);
    for (tok = strtok(line, " \t\r\n"); tok; tok = strtok(NULL, " \t\r\n"))
      test_strlcpy(tok);
  }
  free(line);
}

int
main(int argc, char *argv[])
{
  FILE *fp;
  int i;
  if (argc == 1)
    test_strings_in_file(stdin);
  else
    for (i = 1; i < argc; i++)
      if ((fp = fopen(argv[i], "r")) == NULL)
        fprintf(stderr, "%s: can't open file\n", argv[i]), exit(1);
      else
        test_strings_in_file(fp), fclose(fp);
  return 0;
}
