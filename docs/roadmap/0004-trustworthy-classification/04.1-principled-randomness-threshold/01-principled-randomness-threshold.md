# 04.1.01 — Principled randomness threshold

**Story:** [/docs/roadmap/0004-trustworthy-classification/04.1-principled-randomness-threshold/README.md](/docs/roadmap/0004-trustworthy-classification/04.1-principled-randomness-threshold/README.md)

## Goal

As a maintainer I want the epsilon used to decide "close enough to
8.0" derived from sample size by formula rather than a hand-tuned table.

## Work

- Replace `estimated_epsilon` with the expected finite-sample bias of the
  plug-in entropy estimator for 256 symbols (Miller–Madow correction):
  `bias(N) ≈ 255 / (2 · N · ln 2)` bits, multiplied by a safety factor
  (default 4) to cover the variance.
- Keep the old table reachable via a `LegacyThreshold` policy for comparison
  during one release.

## Success criteria

- For N in {1 KiB, 1 MiB, 64 MiB, 1 GiB} of OS-provided random bytes the
  classifier reports `Encrypted` in at least 99 of 100 trials.
- For the same N of gzip-compressed English text it reports `Encrypted` in
  at most 1 of 100 trials.

## Required tests
