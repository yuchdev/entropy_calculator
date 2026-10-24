# 04.4.01 — Classifier

**Story:** [/docs/roadmap/0004-trustworthy-classification/04.4-classifier/README.md](/docs/roadmap/0004-trustworthy-classification/04.4-classifier/README.md)

## Goal

As a user I want a single verdict with a confidence and the
evidence behind it, replacing the three-way `Plain/Binary/Encrypted` enum.

## Work

- `entropy/classifier.h`:
  ```cpp
  enum class DataClass { Empty, Text, Structured, Compressed, Encrypted,
                         Random, Unknown };
  struct Verdict {
      DataClass data_class;
      double confidence;            // 0..1
      std::optional<Magic> format;  // from 04.3
      Statistics stats;             // from 04.2
      std::vector<std::string> evidence;  // human-readable reasons
  };
  Verdict classify(const Statistics&, std::optional<Magic>);
  ```
- Decision rules (documented in the header, in order):
  1. `sample_size == 0` → `Empty`.
  2. Magic says compressed/executable/image/… → that category, confidence
     by whether the statistics agree (e.g. gzip body should be near 8).
  3. Entropy ≥ 8 − threshold(N), chi-square p in (0.001, 0.999),
     |serial corr| < 0.05 → `Encrypted` (or `Random` if the caller marks the
     source as a generator rather than a file).
  4. Entropy ≥ 7.5 but any statistical test fails → `Compressed`.
  5. Entropy ≤ 5.5 and ≥ 90 % of bytes printable ASCII → `Text`.
  6. Otherwise `Structured`.
- `Encrypted` vs `Random` is the caller's hint, not a statistical result;
  document this honestly.
- Keep `ShannonEncryptionChecker::information_entropy_estimation` as a
  deprecated adapter mapping `Verdict` back to the old enum.

## Success criteria

- Corpus test (04.5) reaches at least 95 % accuracy on the labelled fixtures.
- Each verdict carries at least one evidence string.

## Required tests
