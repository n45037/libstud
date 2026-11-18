# libstud

A small, allocator-agnostic C99 utility library. Every allocation and raw
memory operation goes through a swappable function table, so consumers can
plug in their own `malloc`/`realloc`/`free` (or a custom arena, tracking
allocator, etc.) with a single call.

## Features

### Available now

- **`String`** (`lib/string`) — a dynamic string type that tracks its own
  length and allocated size. Supports append, substring, remove, replace,
  case conversion, and substring search.
- **Custom allocator override** (`lib/alloc`) — `stud_set_allocator()` lets
  you replace any of `malloc`, `realloc`, `free`, `memcpy`, `memmove`, or
  `memcmp` globally. Every data structure in the library goes through these,
  so one call reconfigures everything.
- **Utils** (`lib/utils`) — internal helpers shared across the library
  (`stud_memmem`, allocation-failure handlers).

### Planned

- `Vector` (dynamic array)
- `LinkedList`
- `Queue`
- `Set`
- `HashMap`

## Building

```sh
make            # build compiled/main
make debug      # ASan debug build
make test       # build and run the unit tests
make release    # build compiled/libstud.so
```

## Using it in your own project

Link against the shared library and include the headers you need:

```sh
gcc yourprog.c -Icompiled/include -Lcompiled -lstud -o yourprog
```

```c
#include "alloc.h"
#include "string.h"

int main(void) {
  char src[] = "hello";
  String *s = string_new(src, sizeof(src));
  string_append(s, ", world!", sizeof(", world!"));
  string_free(s);
  return 0;
}
```

To use your own allocator instead of the libc defaults:

```c
StudAllocator custom = {
  .malloc_fn = my_malloc,
  .realloc_fn = my_realloc,
  .free_fn = my_free,
};
stud_set_allocator(custom); /* unset fields keep their current implementation */
```

## Project layout

```
lib/
  alloc/   allocator override plumbing
  string/  dynamic String type
  utils/   internal shared helpers
```

## License

MIT © 2026 Artemii \<forkyou\> F.
