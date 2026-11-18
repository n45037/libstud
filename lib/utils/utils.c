#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../alloc/alloc.h"

#include "utils.h"

void stud_malloc_failure(void) {
  perror("malloc");
  exit(EXIT_FAILURE);
}

void stud_realloc_failure(void) {
  perror("realloc");
  exit(EXIT_FAILURE);
}

void *stud_memmem(const void *haystack, size_t hlen, const void *needle,
                  size_t nlen) {
  if (nlen == 0)
    return (void *)haystack;

  if (hlen < nlen)
    return NULL;

  const char *h = haystack;
  for (size_t i = 0; i <= hlen - nlen; i++)
    if (stud_memcmp(h + i, needle, nlen) == 0)
      return (void *)(h + i);

  return NULL;
}
