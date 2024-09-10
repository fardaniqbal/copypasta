/* Public domain implementation of xmalloc/xrealloc/xcalloc.  These macros
   behave like their corresponding standard C functions, except they crash on
   allocation failure with a message indicating file and line of the crash. */
#include <stdlib.h>

#define xmalloc(sz) \
  xrealloc_(NULL, sz, __FILE__, __LINE__, "xmalloc(" #sz ")")
#define xrealloc(ptr, sz) \
  xrealloc_(ptr, sz, __FILE__, __LINE__, "xrealloc(" #ptr ", " #sz ")")
#define xcalloc(n, sz) \
  xcalloc_(n, sz, __FILE__, __LINE__, "xcalloc(" #n ", " #sz ")")

void *xrealloc_(void *, size_t, const char *file, int line, const char *expr);
void *xcalloc_(size_t, size_t, const char *file, int line, const char *expr);

/* - can put the above in a header and the below in a C/C++ file ----------- */

#include <limits.h> /* for LLONG_MAX */
#include <stdio.h>

#ifdef LLONG_MAX /* "%llu" seems to be more portable than "%zu" */
# if defined _MSC_VER || (defined __MINGW32__ && !__USE_MINGW_ANSI_STDIO)
#   define XMALLOC_PRINTF_SIZE "I64u"
# else
#   define XMALLOC_PRINTF_SIZE "llu"
# endif
# ifdef __GNUC__
__extension__
# endif
typedef unsigned long long xmalloc_printf_size_t;
#else
# define XMALLOC_PRINTF_SIZE "lu"
typedef unsigned long xmalloc_printf_size_t;
#endif

void *
xrealloc_(void *ptr, size_t sz, const char *file, int line, const char *expr)
{
  if ((ptr = realloc(ptr, sz)) != NULL || sz == 0)
    return ptr;
  fprintf(stderr, "%s:%d: %s failed.\n", file, line, expr);
  fprintf(stderr, "%s:%d: not enough memory to allocate "
          "%" XMALLOC_PRINTF_SIZE " bytes.\n", file, line,
          (xmalloc_printf_size_t) sz);
  exit(1);
}

void *
xcalloc_(size_t n, size_t sz, const char *file, int line, const char *expr)
{
  void *ptr = calloc(n, sz);
  if (ptr != NULL || n == 0 || sz == 0)
    return ptr;
  fprintf(stderr, "%s:%d: %s failed.\n", file, line, expr);
  fprintf(stderr, "%s:%d: not enough memory to allocate "
          "%" XMALLOC_PRINTF_SIZE " x %" XMALLOC_PRINTF_SIZE " bytes.\n",
          file, line, (xmalloc_printf_size_t) n, (xmalloc_printf_size_t) sz);
  exit(1);
}

/* - test ------------------------------------------------------------------ */

int
main(void)
{
  /* TODO */
  return 0;
}
