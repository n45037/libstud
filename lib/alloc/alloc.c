#include "alloc.h"

#include <stdlib.h>
#include <string.h>

static StudAllocator g_alloc = {malloc, realloc, free, memcpy, memmove, memcmp};

#define STUD_SET(field)                                                        \
  if (a.field != NULL)                                                         \
    g_alloc.field = a.field;

void stud_set_allocator(StudAllocator a) {
  STUD_SET(malloc_fn)
  STUD_SET(realloc_fn)
  STUD_SET(free_fn)
  STUD_SET(memcpy_fn)
  STUD_SET(memmove_fn)
  STUD_SET(memcmp_fn)
}

void *stud_malloc(size_t n) { return g_alloc.malloc_fn(n); }

void *stud_realloc(void *p, size_t n) { return g_alloc.realloc_fn(p, n); }

void stud_free(void *p) { g_alloc.free_fn(p); }

void *stud_memcpy(void *dest, const void *src, size_t n) {
  return g_alloc.memcpy_fn(dest, src, n);
}

void *stud_memmove(void *dest, const void *src, size_t n) {
  return g_alloc.memmove_fn(dest, src, n);
}

int stud_memcmp(const void *s1, const void *s2, size_t n) {
  return g_alloc.memcmp_fn(s1, s2, n);
}
