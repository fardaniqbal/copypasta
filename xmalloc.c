/* Public domain implementation of xmalloc/xrealloc/xcalloc.  These macros
   behave like their corresponding standard C functions, except they crash on
   allocation failure with a message indicating file and line of the crash. */
#include <stdio.h>
#include <stdlib.h>

#define xmalloc(sz) \
  xrealloc_(NULL, sz, __FILE__, __LINE__, "xmalloc(" #sz ")")
#define xrealloc(ptr, sz) \
  xrealloc_(ptr, sz, __FILE__, __LINE__, "xrealloc(" #ptr ", " #sz ")")
#define xcalloc(n, sz) \
  xcalloc_(n, sz, __FILE__, __LINE__, "xcalloc(" #n ", " #sz ")")

static void *
xrealloc_(void *ptr, size_t sz, const char *file, int line, const char *expr)
{
  if ((ptr = realloc(ptr, sz)) != NULL || sz == 0)
    return ptr;
  fprintf(stderr, "%s:%d: %s failed.\n", file, line, expr);
  fprintf(stderr, "%s:%d: not enough memory to allocate %lu bytes.\n",
         file, line, (unsigned long) sz);
  exit(1);
}

static void *
xcalloc_(size_t n, size_t sz, const char *file, int line, const char *expr)
{
  void *ptr = calloc(n, sz);
  if (ptr != NULL || n == 0 || sz == 0)
    return ptr;
  fprintf(stderr, "%s:%d: %s failed.\n", file, line, expr);
  fprintf(stderr, "%s:%d: not enough memory to allocate %lu x %lu bytes.\n",
         file, line, (unsigned long) n, (unsigned long) sz);
  exit(1);
}
typedef int xmalloc_unused_[!!sizeof &xrealloc_ + !!sizeof &xcalloc_];

/* - test ------------------------------------------------------------------ */

int
main(void)
{
  /* TODO */
  return 0;
}
