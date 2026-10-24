# 06.4.01 — Documentation

**Story:** [/docs/roadmap/0006-packaging-and-release/06.4-documentation/README.md](/docs/roadmap/0006-packaging-and-release/06.4-documentation/README.md)

## Goal

As a new user I want to understand what the numbers mean and how
to use the tool in the common security scenarios.

## Work

- Rewrite README: install, three usage recipes (single file report, locate
  encrypted region, batch scan to JSON), interpretation guide for each
  statistic, limitations (cannot distinguish strong encryption from true
  randomness; compressed-then-encrypted looks encrypted; small samples are
  unreliable).
- `docs/output_schema.md` (from 05.2), `docs/statistics.md` with formulas
  and references (Shannon 1948; Miller 1955; Knuth TAOCP vol. 2 for the
  chi-square and serial tests; `ent` by John Walker).
- Doxygen comments on all public headers; optional `docs` target.

## Success criteria

- A reader can reproduce every README example on the committed fixtures.
- Each public symbol has a doc comment (checked by Doxygen warnings-as-errors
  in CI when the `docs` target is enabled).

## Required tests
