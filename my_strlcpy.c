#include <string.h>

/* Implementation of Theo de Raadt's strlcpy() as presented in [1].  Copy up to
   NBYTE-1 bytes from nul-terminated string SRC to DST.  DST is _guaranteed_ to
   be nul-terminated if NBYTE > 0.  Return strlen(SRC).  See de Raadt's paper
   for rationale.
   [1] https://www.sudo.ws/todd/papers/strlcpy.html */
static size_t
my_strlcpy(char *dst, const char *src, size_t nbyte)
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

/* Call my_strlcpy() on source string STR and run various sanity checks. */
static void
test_strlcpy(const char *str)
{
  size_t nbyte, srclen = strlen(str);
  char buf[256];
  assert(my_strlcpy(NULL, str, 0) == srclen);
  for (nbyte = 1; nbyte <= sizeof buf; nbyte++) {
    size_t ret = my_strlcpy(buf, str, nbyte);
    assert(ret == srclen || !!!"strlcpy must return strlen(src)");
    assert(buf[MIN(nbyte-1, srclen)] == '\0' || !!!"missing nul byte");
    assert(memcmp(buf, str, MIN(nbyte-1, srclen)) == 0 || !!!"bad copy");
  }
}

/* Test my_strlcpy() on various strings read from FP. */
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
