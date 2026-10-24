# 01.2.01 — Fix known CLI defects

**Story:** [/docs/roadmap/0001-stabilise/01.2-fix-known-cli-defects/README.md](/docs/roadmap/0001-stabilise/01.2-fix-known-cli-defects/README.md)

## Goal

As a user I want the `normal` and `linear` options to generate the
distribution their names promise, so that generated datasets are correct.

## Work

- In `main.cpp`, swap the branches so `normal` calls
  `generate_normal_distribution` and `linear` calls
  `generate_uniform_distribution`. Rename `linear` to `uniform` in help text
  and accept `linear` as a deprecated alias for one release.
- Chain the branches with `else if` so `normal` no longer falls into
  `usage_exit()`.
- `CommandLineParams::mean()` returns `_mean`, not `_help`.
- Remove the dead `argc == 2` fallback at the end of `main`, or make it the
  documented "bare filename" shortcut and test it.

## Success criteria

- `-r normal -s 1000000 -m 128 -d 20` produces entropy in the range expected
  for that distribution (roughly 5.9–6.1 bits).
- `-r uniform -s 1000000` produces entropy above 7.99.
- `-r normal` without `-s` reports the "sequence size required" error.

## Required tests
