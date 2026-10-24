# 02.2.01 — Chunked file reader with cancellation and progress

**Story:** [/docs/roadmap/0002-histogram-streaming-api/02.2-chunked-file-reader-with-cancellation-and-progress/README.md](/docs/roadmap/0002-histogram-streaming-api/02.2-chunked-file-reader-with-cancellation-and-progress/README.md)

## Goal

As a CLI user I want large files analysed at disk speed with a
responsive progress bar and the ability to cancel with Ctrl+C on every
platform.

## Work

- Replace per-byte `file.read` with `std::ifstream` opened in binary mode
  reading into a 1 MiB heap buffer (`std::vector<uint8_t>`), feeding
  `ByteHistogram::update(ptr, n)`. This removes the dependency on the
  `uint8_t` codecvt for the main path; keep `uint8_codecvt.h` only if any
  other code still needs `basic_ifstream<uint8_t>`.
- Define a per-call context instead of static state:
  ```cpp
  struct ScanOptions {
      std::function<void(uint64_t bytes_done, uint64_t bytes_total)> progress;
      std::shared_ptr<std::atomic<bool>> cancel;   // optional
      size_t chunk_size = 1 << 20;
  };
  ```
- Progress callback fires once per chunk, not per byte.
- Install a SIGINT handler on POSIX (`sigaction`) and keep the existing
  `SetConsoleCtrlHandler` on Windows; both set the shared atomic flag.
- Deprecate `ShannonEncryptionChecker::interrupt()` and the function-pointer
  `set_callback`; keep them as thin forwards for one release, then remove.
- A cancelled scan returns a distinct result (`std::optional` empty or a
  `ScanStatus::Cancelled` enum), not an empty vector.

## Success criteria

- A 1 GiB file is scanned in under 2 s from page cache (Release build).
- Ctrl+C during a scan exits within 200 ms with exit code 130 (POSIX) and
  a "cancelled" message, no partial result printed.
- Progress callback invoked at most `ceil(size / chunk_size)` + 1 times.

## Required tests
