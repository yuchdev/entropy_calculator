# 03.2.01 — Region summarisation

**Story:** [/docs/roadmap/0003-block-entropy/03.2-region-summarisation/README.md](/docs/roadmap/0003-block-entropy/03.2-region-summarisation/README.md)

## Goal

As an analyst I want contiguous blocks of similar entropy merged
into regions, so output for a 1 GiB file is a handful of lines, not 262,144.

## Work

- `std::vector<Region> summarise(const std::vector<BlockResult>&, double
  tolerance)`: merges adjacent blocks whose entropy differs by less than
  `tolerance`; each region carries offset, length, min/mean/max entropy and
  the 0004 class once available.
- Default tolerance 0.25 bits.

## Success criteria

- The three-plateau fixture summarises to exactly three regions at default
  tolerance.

## Required tests
