#ifndef STUD_ALLOC_H
#define STUD_ALLOC_H

#include <stddef.h>

/*
 * alloc.h - swappable allocator / raw-memory-op function table used by
 * every data structure in libstud. Override malloc/realloc/free/memcpy/
 * memmove/memcmp with a single stud_set_allocator() call.
 */

/**
 * Function table passed to stud_set_allocator().
 *
 * Leave a field NULL to keep whatever implementation is currently
 * configured for it (libc's, by default).
 */
typedef struct {
  void *(*malloc_fn)(size_t);
  void *(*realloc_fn)(void *, size_t);
  void (*free_fn)(void *);
  void *(*memcpy_fn)(void *, const void *, size_t);
  void *(*memmove_fn)(void *, const void *, size_t);
  int (*memcmp_fn)(const void *, const void *, size_t);
} StudAllocator;

/**
 * Override the allocator/raw-memory functions used throughout libstud.
 * One call reconfigures every data structure in the library, since they
 * all go through stud_malloc()/stud_realloc()/stud_free()/etc.
 *
 * @param alloc replacement functions; any NULL field is left unchanged,
 *              so you can override just e.g. malloc/realloc/free and
 *              keep the libc memcpy/memmove/memcmp
 *
 * Example:
 *   StudAllocator custom = {
 *     .malloc_fn  = my_malloc,
 *     .realloc_fn = my_realloc,
 *     .free_fn    = my_free,
 *   };
 *   stud_set_allocator(custom);
 */
void stud_set_allocator(StudAllocator alloc);

/**
 * Allocate n bytes with the currently configured allocator.
 *
 * @return pointer to the allocation, or NULL on failure (see
 *         stud_malloc_failure() in utils.h)
 *
 * Example:
 *   void *p = stud_malloc(128);
 *   if (!p) stud_malloc_failure();
 */
void *stud_malloc(size_t n);

/**
 * Resize a block from stud_malloc()/stud_realloc(), with the currently
 * configured allocator.
 *
 * @param p existing block (NULL behaves like stud_malloc())
 * @return pointer to the (possibly moved) block, or NULL on failure
 *
 * Example:
 *   char *tmp = stud_realloc(buf, new_size);
 *   if (!tmp) stud_realloc_failure();
 *   buf = tmp;
 */
void *stud_realloc(void *p, size_t n);

/**
 * Free a block from stud_malloc()/stud_realloc(), with the currently
 * configured allocator. NULL is safe, as with libc free().
 *
 * Example:
 *   stud_free(p);
 */
void stud_free(void *p);

/**
 * memcpy() through the currently configured implementation. Same
 * semantics as libc memcpy() - src and dest must not overlap.
 *
 * Example:
 *   stud_memcpy(dest, src, n);
 */
void *stud_memcpy(void *dest, const void *src, size_t n);

/**
 * memmove() through the currently configured implementation. Same
 * semantics as libc memmove() - safe for overlapping src/dest.
 *
 * Example:
 *   stud_memmove(dest, src, n);
 */
void *stud_memmove(void *dest, const void *src, size_t n);

/**
 * memcmp() through the currently configured implementation. Same
 * semantics as libc memcmp().
 *
 * Example:
 *   if (stud_memcmp(a, b, n) == 0) { ... }
 */
int stud_memcmp(const void *s1, const void *s2, size_t n);

#endif
