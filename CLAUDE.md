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
- There are no tests or lint configuration in the repo, despite the README mentioning Boost Test.

Run: `build/entropy_calculator/entropy_calculator -f <file>` or `-r <linear|normal> -s <size> [-m mean] [-d stddev]`.

## Architecture

Two CMake targets, wired in the top-level `CMakeLists.txt`:

- `entropy` (`entropy/`): library. `shannon_entropy.h` has the header-only template `shannon_entropy(first, last)` over a range of per-byte probabilities, plus the `ShannonEncryptionChecker` class (implemented in `shannon_entropy.cpp`). It reads a file or memory buffer, builds the 256-bin probability vector, computes entropy, estimates the data class (`Plain/Binary/Encrypted/Unknown`) using an epsilon that shrinks with sample size, and computes the theoretical minimum compressed size. Progress is reported through a plain function-pointer callback (`set_callback`); `interrupt()` sets a static flag to stop calculation (wired to Ctrl+C only on Windows in `main.cpp`).
- `entropy_calculator` (`entropy_calculator/`): CLI executable. `main.cpp` dispatches on the options from `command_line_parser.cpp` (Boost.ProgramOptions; help, version, from-file and random-distribution are mutually exclusive) and `random_distributions.cpp` generates test sequences. Progress display uses `boost::progress_display`.

`uint8_codecvt.h` provides a `std::codecvt<uint8_t>` specialisation so `basic_ifstream<uint8_t>` can read binary files. MSVC ships one; GCC/libc++ do not. Anything reading files as `uint8_t` streams depends on it being installed first (`ShannonEncryptionChecker`'s constructor handles this via `load_uint8_codecvt_`).

## Known issues in the existing code

Verify before relying on these:

- `main.cpp`: the `"normal"` branch calls `generate_uniform_distribution` and the `"linear"` branch calls `generate_normal_distribution`, so the names are swapped relative to the CLI help. The first `if` is not chained with `else if`, so `normal` also falls into the final `else` and calls `usage_exit()`.
- `CommandLineParams::mean()` returns `_help` instead of `_mean`.
- The callback overload of `shannon_entropy` uses `assert` without including `<cassert>` and never invokes the callback.
- The version string `0.0.3` is hardcoded in `main.cpp`.
