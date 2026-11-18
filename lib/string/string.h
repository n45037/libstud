#ifndef STUDSTRING_H

#include <stdbool.h>
#include <stdio.h>

#define STUDSTRING_H

/*
 * string.h - a dynamic, heap-allocated string that tracks its own
 * length and buffer size.
 *
 * Two conventions used throughout this file:
 *   - any "_size" parameter (str_size, src_size, dest_size, substr_size)
 *     is the FULL buffer size INCLUDING the terminating '\0' - i.e. what
 *     sizeof() gives you for a string literal/array, or strlen(x) + 1
 *     for a heap pointer.
 *   - preconditions noted below (and passing NULL where a valid pointer
 *     is expected) are enforced with assert(); getting them wrong aborts
 *     the program rather than returning an error.
 */

/**
 * Dynamic string handle returned by string_new().
 *
 * `size` is the buffer size in bytes, including the terminating '\0'.
 * `len` is the character count (size - 1). Prefer the string_len()/
 * string_size() accessors over reading these fields directly.
 */
typedef struct {
  char *str;
  size_t size;
  size_t len;
} String;

/**
 * Create a new String from a NUL-terminated buffer.
 *
 * @param str      source buffer to copy
 * @param str_size full size of str including the terminating '\0';
 *                  must be > 0
 * @return a heap-allocated String; free it with string_free() when done
 *
 * Example:
 *   char src[] = "hello";
 *   String *s = string_new(src, sizeof(src));
 */
String *string_new(const char *str, size_t str_size);

/**
 * Free a String created by string_new(), including its internal buffer.
 *
 * Example:
 *   string_free(s);
 */
void string_free(String *string);

/**
 * @return number of characters in string, not counting the NUL
 *         terminator.
 *
 * Example:
 *   size_t n = string_len(s);
 */
size_t string_len(const String *string);

/**
 * @return size of string's internal buffer in bytes, including the
 *         NUL terminator (i.e. string_len(string) + 1).
 *
 * Example:
 *   size_t n = string_size(s);
 */
size_t string_size(const String *string);

/**
 * Copy string's contents out into a plain, NUL-terminated C buffer.
 *
 * @param dest      destination buffer
 * @param dest_size must be EXACTLY string_size(string) - not just big
 *                  enough - so size dest with string_size() first
 *
 * Example:
 *   char buf[string_size(s)];
 *   string_raw(s, buf, sizeof(buf));
 */
void string_raw(const String *string, char *dest, size_t dest_size);

/**
 * Append src to the end of string, growing its buffer as needed.
 *
 * @param src      NUL-terminated data to append
 * @param src_size full size of src including the terminating '\0';
 *                  must be > 0
 *
 * Example:
 *   char more[] = ", world!";
 *   string_append(s, more, sizeof(more));
 */
void string_append(String *string, const char *src, size_t src_size);

/**
 * Check whether substr occurs anywhere inside string.
 *
 * @param substr      NUL-terminated text to search for
 * @param substr_size full size of substr including the terminating '\0'
 * @return true if found, false otherwise
 *
 * Example:
 *   char needle[] = "quick";
 *   if (string_includes(s, needle, sizeof(needle))) { ... }
 */
bool string_includes(const String *string, const char *substr,
                     size_t substr_size);

/**
 * @param idx 0-based character index; must be < string_len(string)
 * @return the character at idx
 *
 * Example:
 *   char c = string_at(s, 0); // first character
 */
char string_at(const String *string, size_t idx);

/**
 * Copy a substring of string into dest, NUL-terminated.
 *
 * @param start_idx byte offset to start copying from; must be
 *                  <= string_size(string)
 * @param dest_size length of the copied substring plus 1 (i.e.
 *                  dest_size - 1 characters get copied); must satisfy
 *                  dest_size <= string_size(string) - start_idx
 *
 * Example:
 *   char mid[6];
 *   string_substr(s, 7, mid, sizeof(mid)); // copies 5 chars from idx 7
 */
void string_substr(const String *string, size_t start_idx, char *dest,
                   size_t dest_size);

/**
 * Remove the inclusive range [start_idx, end_idx] from string, shifting
 * what's left and shrinking the buffer.
 *
 * @param start_idx first character to remove (0-based)
 * @param end_idx   last character to remove (0-based, inclusive); must
 *                  be < string_len(string), and start_idx <= end_idx
 *
 * Example:
 *   string_remove(s, 5, 11); // "hello, world!" -> "hello!"
 */
void string_remove(String *string, size_t start_idx, size_t end_idx);

/**
 * Replace the inclusive range [start_idx, end_idx] with src, growing or
 * shrinking string's buffer as needed.
 *
 * @param start_idx first character to replace (0-based)
 * @param end_idx   last character to replace (0-based, inclusive); must
 *                  be < string_len(string)
 * @param src       NUL-terminated replacement text
 * @param src_size  full size of src including the terminating '\0';
 *                  must be > 1
 *
 * Example:
 *   char rep[] = "there";
 *   string_replace(s, 7, 11, rep, sizeof(rep)); // "world" -> "there"
 */
void string_replace(String *string, size_t start_idx, size_t end_idx,
                    const char *src, size_t src_size);

/**
 * Uppercase the inclusive range [start_idx, end_idx] in place.
 *
 * @param end_idx must be < string_len(string) and >= start_idx
 *
 * Example:
 *   string_touppercase(s, 0, string_len(s) - 1); // whole string
 */
void string_touppercase(String *string, size_t start_idx, size_t end_idx);

/**
 * Lowercase the inclusive range [start_idx, end_idx] in place.
 *
 * @param end_idx must be < string_len(string) and >= start_idx
 *
 * Example:
 *   string_tolowercase(s, 0, string_len(s) - 1); // whole string
 */
void string_tolowercase(String *string, size_t start_idx, size_t end_idx);

#endif
