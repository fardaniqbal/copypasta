/* Public domain implementation of bsearch_r, a reentrant version of
   standard C's bsearch function. */
#include <stdlib.h>

/* Binary search for TARGET in CNT-length array ARR of NBYTE-size elements.
   Return an element that equals TARGET according to 3-way comparison
   COMPARE, or NULL if not found.  Assume ARR is sorted ascendingly.  AUX
   can specify optional context data to pass to COMPARE. */
static void *
bsearch_r(const void *target, const void *arr, size_t cnt, size_t nbyte,
          int (*compare)(const void *, const void *, void *aux), void *aux)
{
  size_t lo = 0, hi = cnt;
  while (lo < hi) {
    size_t mid = lo + (hi - lo) / 2;
    int res = compare(target, (const char *) arr + mid * nbyte, aux);
    if (res < 0)
      hi = mid;
    else if (res > 0)
      lo = mid + 1;
    else
      return (char *) arr + mid * nbyte; /* discard const */
  }
  return NULL;
}

/* - test -------------------------------------------------------------- */

#undef NDEBUG
#include <assert.h>
#include <stdio.h>

static int
int_cmp(const void *a, const void *b, void *aux)
{
  (void) aux;
  /* Don't just subtract the arguments because that might overflow. */
  return (*(int *) a < *(int *) b ? -1 :
          *(int *) a > *(int *) b ?  1 : 0);
}

/* Make sure bsearch_r handles empty input array. */
static void
test_bsearch_empty(void)
{
  int tgt = 42;
  assert(bsearch_r(&tgt, NULL, 0, sizeof tgt, int_cmp, NULL) == NULL);
  assert(bsearch_r(NULL, NULL, 0, sizeof tgt, int_cmp, NULL) == NULL);
  assert(bsearch_r(&tgt, &tgt, 0, sizeof tgt, int_cmp, NULL) == NULL);
  assert(bsearch_r(NULL, &tgt, 0, sizeof tgt, int_cmp, NULL) == NULL);
  /* Passing element size 0 is undefined so don't have to test for it. */
}

/* Make sure that bsearch_r finds TARGET in sorted array ARR if ARR
   actually contains TARGET.  If TARGET is not in ARR, then make sure that
   bsearch returns NULL. */
static void
test_bsearch(int target, const int *arr, size_t cnt)
{
  int *res = bsearch_r(&target, arr, cnt, sizeof *arr, int_cmp, NULL);
  size_t i;
  if (res == NULL) {
    for (i = 0 ; i < cnt; i++)
      assert(arr[i] != target || !!!"search did not find target");
  } else {
    assert((arr <= res && res < arr+cnt) || !!!"search returned bad ptr");
    assert(*res == target || !!!"wrong search result");
  }
}

static int
is_sorted(const void *arr, size_t cnt, size_t nbyte,
          int (*compare)(const void *, const void *, void *), void *aux)
{
  const char *base = (const char *) arr;
  size_t i;
  for (i = 1; i < cnt; i++)
    if (compare(base + (i-1) * nbyte, base + i * nbyte, aux) > 0)
      return 0;
  return 1;
}

int
main(void)
{
  const int arr[] = {
    1, 1, 3, 5, 6, 7, 10, 11, 11, 11, 12, 13, 14, 20, 23, 24, 25, 26, 27,
    28, 29, 30, 31, 32, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 49,
    51, 53, 55, 56, 57, 58, 59, 60, 61, 63, 64, 66, 66, 67, 68, 69, 70, 71,
    72, 73, 74, 75, 76, 77, 78, 79, 80, 88, 89, 90, 91, 92, 93, 94, 95, 96,
    96, 96, 96, 96, 97, 99
  };
  size_t cnt = sizeof arr / sizeof *arr;
  size_t start, n, target;

  assert(is_sorted(arr, cnt, sizeof *arr, int_cmp, 0));
  test_bsearch_empty();

  for (start = 0; start < cnt/2; start++)
    for (n = 0; n < cnt-start; n++)
      for (target = 0; target <= 100; target++)
        test_bsearch(target, arr + start, n);

  fprintf(stderr, "bsearch_r passed all tests\n");
  return 0;
}
