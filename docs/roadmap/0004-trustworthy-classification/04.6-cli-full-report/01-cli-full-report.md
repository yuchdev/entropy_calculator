# 04.6.01 — CLI: full report

**Story:** [/docs/roadmap/0004-trustworthy-classification/04.6-cli-full-report/README.md](/docs/roadmap/0004-trustworthy-classification/04.6-cli-full-report/README.md)

## Goal

As a CLI user I want the default single-file output to show the
verdict, confidence, detected format and the statistical table.

## Work

- Default output becomes:
  ```
  File:        sample.bin
  Size:        1,048,576 bytes
  Entropy:     7.999812 bits/byte
  Chi-square:  248.3 (p = 0.61)
  Mean:        127.49
  Serial corr: 0.0004
  Format:      none detected
  Verdict:     Encrypted (confidence 0.97)
  Evidence:    entropy within 4σ of 8.0 for N=1 MiB; chi-square consistent
               with uniform; no compression signature
  Min compressed size: 1,048,551 bytes
  ```
- `--legacy` reproduces the old three-line output for scripts that parse it.

## Success criteria

- Golden-output test passes for three fixtures (random, gzip, text).

## Required tests
