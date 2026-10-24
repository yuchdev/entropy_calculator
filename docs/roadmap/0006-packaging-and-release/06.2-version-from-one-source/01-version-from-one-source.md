# 06.2.01 — Version from one source

**Story:** [/docs/roadmap/0006-packaging-and-release/06.2-version-from-one-source/README.md](/docs/roadmap/0006-packaging-and-release/06.2-version-from-one-source/README.md)

## Goal

As a maintainer I want the version defined once.

## Work

- `project(entropy_calculator VERSION x.y.z)` in the top-level CMake file;
  `configure_file` generates `version.h` with the string and components.
- `--version` prints the version, build type, compiler and Boost version.
- Git tag on each release; CI builds release artifacts for the three
  platforms on tag push.

## Success criteria

- `--version` output equals the tag that produced the binary.

## Required tests
