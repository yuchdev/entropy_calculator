# 01.3.01 — Fix library defects and header hygiene

**Story:** [/docs/roadmap/0001-stabilise/01.3-fix-library-defects-and-header-hygiene/README.md](/docs/roadmap/0001-stabilise/01.3-fix-library-defects-and-header-hygiene/README.md)

## Goal

As a library consumer I want headers that compile cleanly in
isolation and a callback overload that actually calls back.

## Work

- Add `#include <cassert>` and `#include <cstdint>` to `shannon_entropy.h`.
- Either make the callback overload of `shannon_entropy` invoke `callback`
  per element or delete it. Decision: delete it; progress belongs to the
  histogram-building step (0002), not to the 256-element reduction.
- Remove `std::move` on returned locals (pessimises copy elision).
- Replace `std::locale::global(...)` side effect in the constructor with an
  imbue on the specific stream, so constructing a checker no longer mutates
  process-wide locale state.
- Fix `read_file_probabilities` reading one extra byte after EOF: the loop
  tests `eof()` before `read()`, so the final iteration increments a bin with
  a stale value. Use `while (file.read(...))` or chunked reads (02.2).

## Success criteria

- Each public header compiles in a translation unit that includes nothing
  else (`-Wall -Wextra -Werror` on GCC/Clang, `/W4 /WX` on MSVC).
- Entropy of a file equals entropy of the same bytes passed as a buffer, bit
  for bit, for every fixture.

## Required tests
