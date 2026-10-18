# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

C++ tool computing Shannon entropy (0.0–8.0 bits/byte, no leading coefficient) of a file or of a generated random distribution. Used to guess whether data is plain, binary, compressed or encrypted (encrypted ≈ 8.0). `script/file_entropy.py` is a standalone Python reference implementation, not part of the CMake build.

## Build

```
mkdir build && cd build && cmake .. && cmake --build .
```

- Requires CMake 3.12+, Boost >= 1.61 (headers plus `program_options` and `filesystem` for the executable) and Threads.
- Boost location: set `MY_BOOST_DIR` (becomes `BOOST_ROOT`); on Windows `WINDOWS_BOOST_DIR` is mapped to it. Boost is linked statically (`Boost_USE_STATIC_LIBS ON`).
- Clang builds force `-stdlib=libc++`.
- There are no tests or lint configuration in the repo, despite the README mentioning Boost Test, so there is no single-test command. Verify changes by running the executable (e.g. `-f` on a known file, or compare against `script/file_entropy.py`).

Run: `build/entropy_calculator/entropy_calculator -f <file>` or `-r <linear|normal> -s <size> [-m mean] [-d stddev]`.

## Architecture

Source layout: each target keeps public headers in `include/<target>/` and sources in `src/` (e.g. `entropy/include/entropy/shannon_entropy.h`, `entropy_calculator/src/main.cpp`).

Two CMake targets, wired in the top-level `CMakeLists.txt`:

- `entropy` (`entropy/`): library. `shannon_entropy.h` has the header-only template `shannon_entropy(first, last)` over a range of per-byte probabilities, plus the `ShannonEncryptionChecker` class (implemented in `shannon_entropy.cpp`). It reads a file or memory buffer, builds the 256-bin probability vector, computes entropy, estimates the data class (`Plain/Binary/Encrypted/Unknown`) using an epsilon that shrinks with sample size, and computes the theoretical minimum compressed size. Progress is reported through a plain function-pointer callback (`set_callback`); `interrupt()` sets a static flag to stop calculation (wired to Ctrl+C only on Windows in `main.cpp`).
- `entropy_calculator` (`entropy_calculator/`): CLI executable. `main.cpp` dispatches on the options from `command_line_parser.cpp` (Boost.ProgramOptions; help, version, from-file and random-distribution are mutually exclusive) and `random_distributions.cpp` generates test sequences. Progress display uses `boost::progress_display`.

`uint8_codecvt.h` provides a `std::codecvt<uint8_t>` specialisation so `basic_ifstream<uint8_t>` can read binary files. MSVC ships one; GCC/libc++ do not. Anything reading files as `uint8_t` streams depends on it being installed first (`ShannonEncryptionChecker`'s constructor handles this via `load_uint8_codecvt_`).

## Notes

- The callback overload of `shannon_entropy` calls `callback.init(n)` once and `callback(++counter)` per item, so the callback object needs both `init(uintmax_t)` and `operator()(uintmax_t)`. Nothing in the repo uses this overload; the CLI uses `ShannonEncryptionChecker::set_callback` instead.
- The version string lives in the `VERSION` constant in `main.cpp`.
- Several files are committed with CRLF line endings while `core.autocrlf=true`, so plain `git diff` can show whole-file changes; use `git diff --ignore-space-at-eol`.
