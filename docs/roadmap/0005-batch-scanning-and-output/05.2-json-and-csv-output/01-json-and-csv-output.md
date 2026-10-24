# 05.2.01 — JSON and CSV output

**Story:** [/docs/roadmap/0005-batch-scanning-and-output/05.2-json-and-csv-output/README.md](/docs/roadmap/0005-batch-scanning-and-output/05.2-json-and-csv-output/README.md)

## Goal

As an integrator I want `--format json|jsonl|csv|text` with a
stable, documented schema.

## Work

- `jsonl`: one JSON object per line per file (streams well, survives a crash
  midway). `json`: an array, emitted at the end. `csv`: header row plus one
  row per file, RFC 4180 quoting.
- Schema version field (`"schema": 1`) in every object; documented in
  `docs/output_schema.md` with an example.
- Block mode (0003) nests `regions` as an array inside the file object.
- Write a minimal JSON emitter (strings, numbers, bools, null, arrays,
  objects) in the CLI; no JSON library dependency. Numbers printed with
  `%.17g` so they round-trip.
- Progress and diagnostics go to `stderr` only, so `stdout` is pure data.

## Success criteria

- Output of every format parses with a standard parser (`python3 -c
  "import json,sys; ..."` and the `csv` module) in CI.
- A file name containing quotes, commas, newlines and non-ASCII bytes
  round-trips through each format.

## Required tests
