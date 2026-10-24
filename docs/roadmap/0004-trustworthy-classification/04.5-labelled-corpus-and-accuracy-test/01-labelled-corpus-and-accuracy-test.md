# 04.5.01 — Labelled corpus and accuracy test

**Story:** [/docs/roadmap/0004-trustworthy-classification/04.5-labelled-corpus-and-accuracy-test/README.md](/docs/roadmap/0004-trustworthy-classification/04.5-labelled-corpus-and-accuracy-test/README.md)

## Goal

As a maintainer I want a small labelled corpus so classification
changes are measured, not guessed.

## Work

- `tests/corpus/` with ~40 files under 256 KiB each, generated at configure
  time where possible (text from a committed public-domain excerpt;
  compressed variants produced by the test itself using a vendored tiny
  deflate implementation or by shelling out only when available; random from
  a seeded PRNG; a few committed real headers for ELF/PE/PNG/JPEG/PDF).
- `corpus.csv` with `path,expected_class`.
- Test iterates the corpus, prints a confusion matrix, and asserts the
  accuracy floor.

## Success criteria

- Accuracy ≥ 95 %; confusion matrix printed in CI logs.

## Required tests
