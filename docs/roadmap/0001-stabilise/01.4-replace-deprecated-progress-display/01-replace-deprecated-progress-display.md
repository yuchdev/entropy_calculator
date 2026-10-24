# 01.4.01 — Replace deprecated `boost::progress_display`

**Story:** [/docs/roadmap/0001-stabilise/01.4-replace-deprecated-progress-display/README.md](/docs/roadmap/0001-stabilise/01.4-replace-deprecated-progress-display/README.md)

## Goal

As a maintainer I want the CLI to build against current Boost
(1.72+ removed `boost/progress.hpp`).

## Work

- Implement a small `ConsoleProgress` class in the CLI (`progress.h`) that
  prints a percentage or bar to `stderr` at most every 100 ms.
- Progress goes to `stderr` so `stdout` stays clean for machine output (0005).

## Success criteria

- Builds against Boost 1.61 and the latest Boost release.
- Progress output is absent when `stdout` is not a TTY or `--quiet` is set.

## Required tests
