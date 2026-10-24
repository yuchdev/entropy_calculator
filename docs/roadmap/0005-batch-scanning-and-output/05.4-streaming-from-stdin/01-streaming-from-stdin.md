# 05.4.01 — Streaming from stdin

**Story:** [/docs/roadmap/0005-batch-scanning-and-output/05.4-streaming-from-stdin/README.md](/docs/roadmap/0005-batch-scanning-and-output/05.4-streaming-from-stdin/README.md)

## Goal

As a pipeline author I want `cat dump | entropy_calculator -`
to analyse stdin without a temporary file.

## Work

- `-` as the path reads stdin in binary mode (set `O_BINARY` on Windows).
- Block mode works on stdin; total size is unknown so progress is shown as
  bytes processed rather than a percentage.

## Success criteria

- `head -c 1048576 /dev/urandom | entropy_calculator - --format json` prints
  a verdict of `Encrypted`/`Random`.

## Required tests
