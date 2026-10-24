# 03.3.01 — CLI: `--blocks` mode

**Story:** [/docs/roadmap/0003-block-entropy/03.3-cli-blocks-mode/README.md](/docs/roadmap/0003-block-entropy/03.3-cli-blocks-mode/README.md)

## Goal

As a CLI user I want `entropy_calculator -f file --blocks 4096`
to print a table of regions, and `--blocks 4096 --raw` to print every block.

## Work

- New options: `--blocks <size>`, `--stride <size>`, `--raw`,
  `--tolerance <bits>`.
- Human output: aligned table with offset (hex and decimal), length, entropy,
  and a 20-character bar made of block characters so plateaus are visible in
  a terminal.
- Accepts size suffixes `k`, `m` (`--blocks 64k`).

## Success criteria

- Running on the three-plateau fixture prints three region rows.
- `--blocks` without `-f` is rejected with a clear message.

## Required tests
