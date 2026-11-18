#ifndef STUDUTILS_H

#include <stdio.h>

#define STUDUTILS_H

/*
 * utils.h - small helpers shared across libstud: allocation-failure
 * handlers and a portable memmem() implementation (built on the
 * overridable stud_memcmp(), see alloc.h). Mostly used internally, but
 * fine to call directly too.
 */

/**
 * Report a failed stud_malloc() call and terminate the program.
 *
 * Prints the reason via perror("malloc") (relies on errno, so call this
 * right after the failing allocation) and exits with EXIT_FAILURE. Never
 * returns.
 *
 * Example:
 *   void *p = stud_malloc(n);
 *   if (!p) stud_malloc_failure();
 */
void stud_malloc_failure(void);

/**
 * Report a failed stud_realloc() call and terminate the program.
 *
 * Same behaviour as stud_malloc_failure(), but reports via
 * perror("realloc"). Never returns.
 *
 * Example:
 *   char *tmp = stud_realloc(p, n);
 *   if (!tmp) stud_realloc_failure();
 */
void stud_realloc_failure(void);

/**
 * Find the first occurrence of needle inside haystack (like glibc's
 * memmem()). Uses the currently configured memcmp internally.
 *
 * @param haystack buffer to search
 * @param hlen     length of haystack in bytes
 * @param needle   byte sequence to search for
 * @param nlen     length of needle in bytes
 * @return pointer to the first match in haystack; haystack itself if
 *         nlen == 0; NULL if needle isn't found (or is longer than
 *         haystack)
 *
 * Example:
 *   const char *hay = "hello, world!";
 *   void *hit = stud_memmem(hay, strlen(hay), "world", 5);
 */
void *stud_memmem(const void *haystack, size_t hlen, const void *needle,
                  size_t nlen);

#endif
