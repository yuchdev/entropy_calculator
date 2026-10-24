# 0002 — Histogram and streaming API

**Depends on:** 0001-stabilise

Purpose: make the 256-bin byte histogram a public, incremental type so that
data can be fed from files, memory, sockets or memory-mapped regions, in one
or many passes, and so that later milestones (block entropy, statistical
tests) share one pass over the bytes.

## Stories

| Story | Title | Spec |
|-------|-------|------|
| 02.1 | Public `ByteHistogram` type | [/docs/roadmap/0002-histogram-streaming-api/02.1-public-bytehistogram-type/README.md](/docs/roadmap/0002-histogram-streaming-api/02.1-public-bytehistogram-type/README.md) |
| 02.2 | Chunked file reader with cancellation and progress | [/docs/roadmap/0002-histogram-streaming-api/02.2-chunked-file-reader-with-cancellation-and-progress/README.md](/docs/roadmap/0002-histogram-streaming-api/02.2-chunked-file-reader-with-cancellation-and-progress/README.md) |
| 02.3 | `ShannonEncryptionChecker` re-based on `ByteHistogram` | [/docs/roadmap/0002-histogram-streaming-api/02.3-rebase-checker-on-bytehistogram/README.md](/docs/roadmap/0002-histogram-streaming-api/02.3-rebase-checker-on-bytehistogram/README.md) |
| 02.4 | Micro-benchmark target | [/docs/roadmap/0002-histogram-streaming-api/02.4-micro-benchmark-target/README.md](/docs/roadmap/0002-histogram-streaming-api/02.4-micro-benchmark-target/README.md) |

Back to: [/docs/roadmap/README.md](/docs/roadmap/README.md)
