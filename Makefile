CC=gcc
CFLGAS=-Wall -Werror -Wextra -std=c99 -pedantic
DFLAGS=-fsanitize=address -g -O0 -Wsign-compare -Wtype-limits
SRC=main.c lib/string/string.c lib/utils/utils.c lib/alloc/alloc.c
OUT=compiled/main

LIBSRC=lib/string/string.c lib/utils/utils.c lib/alloc/alloc.c
LIBOUT=compiled/libstud.so

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

debug:
	@$(CC) $(CFLAGS) $(DFLAGS) $(SRC) -o $(OUT)

test:
	$(CC) $(CFLAGS) $(DFLAGS) \
	        lib/string/test.c lib/string/string.c lib/utils/utils.c lib/alloc/alloc.c \
	        -o compiled/tests/string_test
	./compiled/tests/string_test

release: $(LIBOUT)

$(LIBOUT): $(LIBSRC)
	$(CC) $(CFLAGS) -fPIC -shared $(LIBSRC) -o $(LIBOUT)

.PHONY: all debug test release
