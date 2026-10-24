# 04.2.01 — Statistical test battery

**Story:** [/docs/roadmap/0004-trustworthy-classification/04.2-statistical-test-battery/README.md](/docs/roadmap/0004-trustworthy-classification/04.2-statistical-test-battery/README.md)

## Goal

As an analyst I want the tests popularised by the `ent` utility —
chi-square, arithmetic mean, serial correlation, and optionally Monte Carlo
π — computed in the same pass as the histogram.

## Work

- New header `entropy/statistics.h`:
  ```cpp
  struct Statistics {
      double entropy;              // bits/byte
      double chi_square;           // vs uniform, 255 d.o.f.
      double chi_square_p_value;   // probability a uniform source exceeds it
      double arithmetic_mean;      // 127.5 for uniform
      double serial_correlation;   // ~0 for random
      std::optional<double> monte_carlo_pi; // only if enabled
      uint64_t sample_size;
  };
  class StatisticsAccumulator {  // same update()/merge() shape as ByteHistogram
      void update(const uint8_t*, size_t);
      Statistics finish() const;
  };
  ```
- Chi-square and mean derive from the histogram at `finish()`; serial
  correlation needs the previous byte carried across `update` calls; Monte
  Carlo π needs a 6-byte carry buffer. All are O(1) extra state per
  accumulator.
- Chi-square p-value via the regularised upper incomplete gamma function
  implemented locally (no Boost.Math in the library).
- `merge` is **not** supported for serial correlation across arbitrary
  splits; document that parallel scans (0005) partition by file, not within a
  file.

## Success criteria

- On 1 MiB of OS random bytes: chi-square p-value in (0.01, 0.99) in at least
  95 of 100 trials; |serial correlation| < 0.01; mean within 127.5 ± 0.5.
- On 1 MiB of gzip output: chi-square p-value below 0.001.
- On 1 MiB of ASCII text: serial correlation above 0.1.

## Required tests
