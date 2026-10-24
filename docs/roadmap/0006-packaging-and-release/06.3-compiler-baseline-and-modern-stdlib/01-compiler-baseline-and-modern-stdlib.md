# 06.3.01 — Compiler baseline and modern standard library

**Story:** [/docs/roadmap/0006-packaging-and-release/06.3-compiler-baseline-and-modern-stdlib/README.md](/docs/roadmap/0006-packaging-and-release/06.3-compiler-baseline-and-modern-stdlib/README.md)

## Goal

As a maintainer I want to drop Boost from the library entirely.

## Work

- Raise minimum to C++17; replace `boost::filesystem` with
  `std::filesystem` in the library and CLI.
- Keep Boost.ProgramOptions in the CLI (it earns its keep) or replace with a
  small vendored parser; decide based on the Windows static-link experience
  gathered in 0001.
- Remove `uint8_codecvt.h` once no code path reads `basic_ifstream<uint8_t>`.

## Success criteria

- Library target has zero link dependencies beyond the standard library and
  Threads.
- All tests pass on the three platforms.

## Required tests
