# 02.3.01 — `ShannonEncryptionChecker` re-based on `ByteHistogram`

**Story:** [/docs/roadmap/0002-histogram-streaming-api/02.3-rebase-checker-on-bytehistogram/README.md](/docs/roadmap/0002-histogram-streaming-api/02.3-rebase-checker-on-bytehistogram/README.md)

## Goal

As an existing library user I want the current public methods to
keep working while they become thin wrappers over the new types.

## Work

- `get_file_entropy` and `get_sequence_entropy` build a `ByteHistogram` and
  return `.entropy()`.
- `min_compressed_size` unchanged in behaviour but computed from `uint64_t`.
- Mark the class `[[deprecated]]` in a later milestone only after 0004
  introduces its replacement (`Classifier`); for now it stays the stable
  facade.

## Success criteria

- All 0001 tests still pass unchanged.
- Public header of the class no longer includes any Boost header.

## Required tests
