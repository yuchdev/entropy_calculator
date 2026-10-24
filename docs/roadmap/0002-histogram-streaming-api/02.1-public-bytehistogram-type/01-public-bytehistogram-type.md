# 02.1.01 — Public `ByteHistogram` type

**Story:** [/docs/roadmap/0002-histogram-streaming-api/02.1-public-bytehistogram-type/README.md](/docs/roadmap/0002-histogram-streaming-api/02.1-public-bytehistogram-type/README.md)

## Goal

As a library consumer I want to accumulate byte counts
incrementally and derive entropy and probabilities from the result.

## Work

- New header `entropy/byte_histogram.h`:
  ```cpp
  class ByteHistogram {
  public:
      void update(const uint8_t* data, size_t size);
      void update(uint8_t byte);
      void merge(const ByteHistogram& other);
      void reset();
      uint64_t total() const;
      uint64_t count(uint8_t byte) const;
      std::array<double, 256> probabilities() const;
      double entropy() const;   // calls shannon_entropy on probabilities()
  };
  ```
- Counts are `uint64_t` so files larger than 4 GiB are safe on all platforms.
- `update(const uint8_t*, size_t)` is the hot path; keep it branch-free and
  allow the compiler to vectorise (plain indexed increments, no callback
  inside the loop).

## Success criteria

- Throughput of `update` on a 1 GiB in-memory buffer is at least 1 GiB/s on
  a 2020-era laptop in Release build (measured by the benchmark in 02.4).
- `merge` of two histograms equals the histogram of the concatenated data.

## Required tests
