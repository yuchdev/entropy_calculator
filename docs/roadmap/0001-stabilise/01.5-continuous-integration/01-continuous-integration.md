# 01.5.01 — Continuous integration

**Story:** [/docs/roadmap/0001-stabilise/01.5-continuous-integration/README.md](/docs/roadmap/0001-stabilise/01.5-continuous-integration/README.md)

## Goal

As a maintainer I want every push built and tested on all three
platforms.

## Work

- Extend the existing pipeline (see commit `0ec911b Pipeline`) or add a
  GitHub Actions matrix: ubuntu-latest (GCC, Clang), macos-latest (Clang),
  windows-latest (MSVC).
- Run `ctest` in each job. Cache Boost.

## Success criteria

- Badge in README reflects the build state of `master`.
- A PR cannot merge with a red build.

## Required tests
