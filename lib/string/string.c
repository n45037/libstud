#define _GNU_SOURCE

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../alloc/alloc.h"
#include "../utils/utils.h"

#include "string.h"

static void string_setsize(String *string, size_t new_size);
static void string_setlen(String *string, size_t new_len);

String *string_new(const char *str, size_t str_size) {
  assert(str_size > 0);

  String *string = stud_malloc(sizeof(String));
  if (!string)
    stud_malloc_failure();

  string->str = stud_malloc(str_size);
  if (!string->str)
    stud_malloc_failure();

  stud_memcpy(string->str, str, str_size);

  string_setsize(string, str_size);
  string_setlen(string, str_size == 0 ? 0 : str_size - 1);
  return string;
}

void string_free(String *string) {
  assert(string != NULL);
  stud_free(string->str);
  stud_free(string);
}

size_t string_len(const String *string) {
  assert(string != NULL);
  return string->len;
}

static void string_setlen(String *string, size_t new_len) {
  string->len = new_len;
}

size_t string_size(const String *string) {
  assert(string != NULL);
  return string->size;
}

static void string_setsize(String *string, size_t new_size) {
  string->size = new_size;
}

void string_raw(const String *string, char *dest, size_t dest_size) {
  assert(string != NULL);
  assert(dest != NULL);
  assert(dest_size > 0);
  assert(dest_size == string_size(string));
  stud_memcpy(dest, string->str, dest_size - 1);
  dest[dest_size - 1] = '\0';
}

void string_append(String *string, const char *src, size_t src_size) {
  assert(string != NULL);
  assert(src != NULL);
  assert(src_size > 0);

  size_t add_len = src_size - 1;
  char *tmp = stud_realloc(string->str, string_size(string) + add_len);
  if (!tmp)
    stud_realloc_failure();

  string->str = tmp;
  stud_memcpy(string->str + string_len(string), src, add_len);

  string_setlen(string, string_len(string) + add_len);
  string_setsize(string, string_size(string) + add_len);
  string->str[string_size(string) - 1] = '\0';
}

bool string_includes(const String *string, const char *substr,
                     size_t substr_size) {
  assert(string != NULL);
  assert(substr != NULL);
  return stud_memmem(string->str, string_size(string), substr,
                     substr_size - 1) != NULL;
}

char string_at(const String *string, size_t idx) {
  assert(string != NULL);
  assert(idx < string_len(string));
  return string->str[idx];
}

void string_substr(const String *string, size_t start_idx, char *dest,
                   size_t dest_size) {
  assert(string != NULL);
  assert(dest != NULL);
  assert((size_t)start_idx <= string_size(string));
  assert(dest_size <= string_size(string) - start_idx);

  stud_memcpy(dest, string->str + start_idx, dest_size - 1);
  dest[dest_size - 1] = '\0';
}

void string_remove(String *string, size_t start_idx, size_t end_idx) {
  assert(string != NULL);
  assert(end_idx < string_len(string));
  assert(start_idx <= end_idx);

  end_idx++; // include char at index
  size_t offset = end_idx - start_idx;
  size_t move = sizeof(char) * (string_size(string) - offset - start_idx);
  stud_memmove(string->str + start_idx, string->str + end_idx, move);

  char *tmp = stud_realloc(string->str, string_size(string) - offset);
  if (!tmp)
    stud_realloc_failure();

  string->str = tmp;
  string_setlen(string, string_len(string) - offset);
  string_setsize(string, string_size(string) - offset);
}

void string_replace(String *string, size_t start_idx, size_t end_idx,
                    const char *src, size_t src_size) {
  assert(string != NULL);
  assert(end_idx < string_len(string));
  assert(src != NULL);
  assert(src_size > 1);

  size_t str_size = string_size(string);

  for (size_t i = 0; i < src_size; i++)
    if (src[i] == '\0') {
      src_size--;
      break;
    }

  end_idx++; // include char at index
  size_t offset = end_idx - start_idx;
  size_t new_size = str_size - offset + src_size;

  if (new_size > str_size) { // resize to fit new content
    char *tmp = stud_realloc(string->str, new_size);
    if (!tmp)
      stud_realloc_failure();
    string->str = tmp;
    string_setsize(string, new_size);
  }

  stud_memmove(string->str + (start_idx + src_size),
               string->str + (start_idx + offset),
               str_size - offset - start_idx);

  stud_memcpy(string->str + start_idx, src, src_size);
  string->str[new_size - 1] = '\0';

  if (new_size < str_size) { // resize to free extra space that's left
    char *tmp = stud_realloc(string->str, new_size);
    if (!tmp)
      stud_realloc_failure();
    string->str = tmp;
    string_setsize(string, new_size);
  }

  size_t new_len = string_len(string) - offset + src_size;
  string_setlen(string, new_len);
}

void string_touppercase(String *string, size_t start_idx, size_t end_idx) {
  assert(string != NULL);
  assert(end_idx < string_len(string));
  assert(end_idx >= start_idx);

  ++end_idx; // include char at index
  for (size_t i = start_idx; i < end_idx; i++) {
    string->str[i] = toupper(string->str[i]);
  }
}

void string_tolowercase(String *string, size_t start_idx, size_t end_idx) {
  assert(string != NULL);
  assert(end_idx < string_len(string));
  assert(end_idx >= start_idx);

  ++end_idx; // include char at index
  for (size_t i = start_idx; i < end_idx; i++) {
    string->str[i] = tolower(string->str[i]);
  }
}
