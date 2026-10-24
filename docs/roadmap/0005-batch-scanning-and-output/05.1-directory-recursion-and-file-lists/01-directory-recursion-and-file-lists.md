# 05.1.01 — Directory recursion and file lists

**Story:** [/docs/roadmap/0005-batch-scanning-and-output/05.1-directory-recursion-and-file-lists/README.md](/docs/roadmap/0005-batch-scanning-and-output/05.1-directory-recursion-and-file-lists/README.md)

## Goal

As a user I want to point the tool at a directory, a glob, or a
list of paths on stdin and get one result per file.

## Work

- `-f` accepts a directory; add `--recursive/-R`, `--include <glob>`,
  `--exclude <glob>`, `--max-size <bytes>`, `--follow-symlinks`.
- `--files-from -` reads newline-separated paths from stdin (works with
  `find ... | entropy_calculator --files-from -`).
- Unreadable files are reported as a row with an error field, not a crash;
  exit code is 0 if all succeeded, 2 if any file failed, 1 for usage errors.
- Use Boost.Filesystem already linked; `std::filesystem` becomes the target
  once the minimum compiler baseline allows (see 06.3).

## Success criteria

- Scanning a fixture tree of 50 files with one unreadable file yields 50 rows,
  one with an error, exit code 2.
- Symlink loops do not hang when `--follow-symlinks` is set (visited-inode
  set).

## Required tests
