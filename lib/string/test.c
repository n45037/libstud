#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "string.h"

static int tests_run = 0;
static int tests_failed = 0;

#define CHECK(cond)                                                            \
  do {                                                                         \
    tests_run++;                                                               \
    if (!(cond)) {                                                             \
      tests_failed++;                                                          \
      printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);                   \
    }                                                                          \
  } while (0)

static bool raw_eq(String *s, const char *expected) {
  char buf[string_size(s)];
  string_raw(s, buf, sizeof(buf));
  return strcmp(buf, expected) == 0;
}

static void test_new_len_size(void) {
  char src[] = "hello";
  String *s = string_new(src, sizeof(src));
  CHECK(string_len(s) == 5);
  CHECK(string_size(s) == 6);
  CHECK(raw_eq(s, "hello"));
  string_free(s);
}

static void test_append(void) {
  char src[] = "hey";
  String *s = string_new(src, sizeof(src));

  char more[] = ", world!";
  string_append(s, more, sizeof(more));
  CHECK(raw_eq(s, "hey, world!"));
  CHECK(string_len(s) == 11);
  CHECK(string_size(s) == 12);

  /* append a single char (edge case: smallest possible src) */
  char bang[] = "!";
  string_append(s, bang, sizeof(bang));
  CHECK(raw_eq(s, "hey, world!!"));

  string_free(s);
}

static void test_includes(void) {
  char src[] = "the quick brown fox";
  String *s = string_new(src, sizeof(src));

  char a[] = "quick";
  char b[] = "slow";
  char c[] = "fox"; /* match at the very end */
  char d[] = "the"; /* match at the very start */

  CHECK(string_includes(s, a, sizeof(a)) == true);
  CHECK(string_includes(s, b, sizeof(b)) == false);
  CHECK(string_includes(s, c, sizeof(c)) == true);
  CHECK(string_includes(s, d, sizeof(d)) == true);

  string_free(s);
}

static void test_at(void) {
  char src[] = "abcde";
  String *s = string_new(src, sizeof(src));
  CHECK(string_at(s, 0) == 'a');
  CHECK(string_at(s, 4) == 'e'); /* last valid index */
  string_free(s);
}

static void test_substr(void) {
  char src[] = "hello, world!";
  String *s = string_new(src, sizeof(src));

  char mid[6];
  string_substr(s, 7, mid, sizeof(mid));
  CHECK(strcmp(mid, "world") == 0);

  char whole[14];
  string_substr(s, 0, whole, sizeof(whole));
  CHECK(strcmp(whole, "hello, world!") == 0);

  /* start_idx at the very end -> empty result, no overflow */
  char empty[1];
  string_substr(s, string_len(s), empty, sizeof(empty));
  CHECK(strcmp(empty, "") == 0);

  string_free(s);
}

static void test_remove(void) {
  char src[] = "hello, world!";
  String *s = string_new(src, sizeof(src));

  string_remove(s, 5, 11); /* remove ", world" -> "hello!" */
  CHECK(raw_eq(s, "hello!"));
  CHECK(string_len(s) == 6);

  string_free(s);

  /* removing a single character */
  char src2[] = "abc";
  s = string_new(src2, sizeof(src2));
  string_remove(s, 1, 1); /* remove "b" -> "ac" */
  CHECK(raw_eq(s, "ac"));
  string_free(s);
}

static void test_replace_same_len(void) {
  char src[] = "hello, world!";
  String *s = string_new(src, sizeof(src));
  char rep[] = "there";
  string_replace(s, 7, 11, rep, sizeof(rep)); /* "world" -> "there", same len */
  CHECK(raw_eq(s, "hello, there!"));
  CHECK(string_len(s) == string_size(s) - 1);
  string_free(s);
}

static void test_replace_grow(void) {
  char src[] = "hi!";
  String *s = string_new(src, sizeof(src));
  char rep[] = "programmer";
  string_replace(s, 0, 1, rep, sizeof(rep)); /* "hi" -> "programmer" */
  CHECK(raw_eq(s, "programmer!"));
  string_free(s);
}

static void test_replace_shrink(void) {
  char src[] = "hello, programmer!";
  String *s = string_new(src, sizeof(src));
  char rep[] = "x";
  string_replace(s, 7, 16, rep, sizeof(rep)); /* "programmer" -> "x" */
  CHECK(raw_eq(s, "hello, x!"));
  string_free(s);
}

static void test_case_conversion(void) {
  char src[] = "hello, world!";
  String *s = string_new(src, sizeof(src));

  string_touppercase(s, 7, 7); /* just the 'w' */
  CHECK(raw_eq(s, "hello, World!"));

  string_touppercase(s, 0, string_len(s) - 1); /* whole string */
  CHECK(raw_eq(s, "HELLO, WORLD!"));

  string_tolowercase(s, 0, string_len(s) - 1);
  CHECK(raw_eq(s, "hello, world!"));

  string_free(s);
}

int main(void) {
  test_new_len_size();
  test_append();
  test_includes();
  test_at();
  test_substr();
  test_remove();
  test_replace_same_len();
  test_replace_grow();
  test_replace_shrink();
  test_case_conversion();

  printf("\n%d/%d tests passed\n", tests_run - tests_failed, tests_run);
  return tests_failed == 0 ? 0 : 1;
}
