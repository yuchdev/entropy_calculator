# 03.1.01 — Block entropy in the library

**Story:** [/docs/roadmap/0003-block-entropy/03.1-block-entropy-in-library/README.md](/docs/roadmap/0003-block-entropy/03.1-block-entropy-in-library/README.md)

## Goal

As a library consumer I want per-block entropy for a file or
buffer, with configurable block size and stride.

## Work

- New header `entropy/block_entropy.h`:
  ```cpp
  struct BlockResult { uint64_t offset; uint64_t length; double entropy; };
  struct BlockOptions { size_t block_size = 4096; size_t stride = 4096; };
  std::vector<BlockResult> block_entropy(const uint8_t* data, size_t size,
                                         BlockOptions);
  std::vector<BlockResult> block_entropy_file(const std::string& path,
                                              BlockOptions, const ScanOptions&);
  ```
- Non-overlapping case (`stride == block_size`): one histogram per block,
  reset between blocks.
- Overlapping case (`stride < block_size`): rolling update — subtract the
  bytes leaving the window, add the bytes entering, recompute entropy. This
  is O(n) rather than O(n·block_size / stride).
- The file variant reads chunks (02.2) and carries window state across chunk
  boundaries; blocks never straddle incorrectly.
- Final partial block is reported with its true `length` and flagged so
  callers can discard it; its entropy is computed from the bytes present.

## Success criteria

- A fixture made of 64 KiB zeros + 64 KiB random + 64 KiB ASCII text yields
  three plateaus (≈0, ≈8, ≈4.5) with transitions located within one block of
  the true boundaries.
- File and buffer variants give identical results for every fixture.
- Overlapping and non-overlapping paths agree when `stride == block_size`.

## Required tests
