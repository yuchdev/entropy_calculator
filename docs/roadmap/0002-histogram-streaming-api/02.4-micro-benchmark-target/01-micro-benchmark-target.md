# 02.4.01 — Micro-benchmark target

**Story:** [/docs/roadmap/0002-histogram-streaming-api/02.4-micro-benchmark-target/README.md](/docs/roadmap/0002-histogram-streaming-api/02.4-micro-benchmark-target/README.md)

## Goal

As a maintainer I want a repeatable throughput measurement so
performance regressions are caught.

## Work

- `bench/` directory, built behind `option(ENTROPY_BUILD_BENCH OFF)`.
- Measures `ByteHistogram::update` on 256 MiB of pseudo-random data and the
  full file-scan path on a temporary file; prints MiB/s.
- Not run in CI by default; a documented manual step.

## Success criteria

- Benchmark runs and prints two throughput numbers.
- README documents how to run it and the reference numbers for one machine.

## Required tests
