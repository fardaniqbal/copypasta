/* Public domain implementation of bsearch_r, a reentrant version of
   standard C's bsearch function. */
#include <stdlib.h>

/* Binary search for TARGET in CNT-length array ARR of NBYTE-size elements.
   Assume ARR is sorted ascendingly according to COMPARE(a, b, AUX), which
   must return negative, zero, or positive if its first argument is
   respectively less than, equal to, or greater than its second argument.
   Like ISO C's bsearch, but with extra argument AUX for reentrancy. */
void *
bsearch_r(const void *target, const void *arr, size_t cnt, size_t nbyte,
          int (*compare)(const void *, const void *, void *), void *aux)
{
  /* TODO */
  return NULL;
}

/* - test -------------------------------------------------------------- */

int
main(void)
{
  int sorted_arr[] = {
    1, 3, 5, 6, 7, 10, 11, 12, 13, 14, 26, 29, 20, 22, 23, 24, 25, 26, 27,
    28, 29, 30, 31, 32, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 49,
    51, 53, 55, 56, 57, 58, 59, 60, 61, 53, 64, 66, 67, 68, 69, 70, 71, 72,
    73, 74, 75, 76, 77, 78, 79, 80, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97
  };
  /* TODO */
  return 0;
}
