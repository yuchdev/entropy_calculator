# 05.3.01 — Parallel scanning

**Story:** [/docs/roadmap/0005-batch-scanning-and-output/05.3-parallel-scanning/README.md](/docs/roadmap/0005-batch-scanning-and-output/05.3-parallel-scanning/README.md)

## Goal

As a user with many files I want the scan to use all cores.

## Work

- `--jobs N` (default: hardware concurrency). A fixed-size thread pool pulls
  paths from a queue; results are collected in input order for `json`/`csv`
  and emitted as ready for `jsonl` with a `"seq"` field.
- Parallelism is per file; a single file is never split, so serial
  correlation (04.2) remains valid.
- Cancellation flag (02.2) is shared across the pool; Ctrl+C stops all
  workers and flushes what is complete.

## Success criteria

- 1,000 small fixture files scan at least 3× faster with `--jobs 4` than
  `--jobs 1` on a 4-core machine (benchmark, not a CI assertion).
- Output is byte-identical between `--jobs 1` and `--jobs 8` for `json` and
  `csv` formats.

## Required tests
