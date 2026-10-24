# 04.3.01 — Format recognition by magic bytes

**Story:** [/docs/roadmap/0004-trustworthy-classification/04.3-format-recognition-by-magic-bytes/README.md](/docs/roadmap/0004-trustworthy-classification/04.3-format-recognition-by-magic-bytes/README.md)

## Goal

As an analyst I want known containers named (gzip, zip, xz, bzip2,
zstd, 7z, PNG, JPEG, PDF, ELF, PE, Mach-O, SQLite, …) so that high entropy
with a recognised compression header is reported as "compressed: gzip" and
not "encrypted".

## Work

- `entropy/magic.h`: a small table of (offset, byte pattern, name, category
  ∈ {Compressed, Executable, Image, Document, Database, Archive, Encrypted
  container}) with a `detect(const uint8_t* head, size_t n)` function
  operating on the first 64 bytes.
- Include a few explicitly encrypted-container signatures (e.g. OpenPGP
  packet headers, LUKS, "Salted__" from OpenSSL enc) in the
  `EncryptedContainer` category.
- Table is data-driven so additions are one line each; no external
  `libmagic` dependency.

## Success criteria

- Every signature in the table has a fixture header and is detected.
- A random 64-byte head matches nothing in 10,000 seeded trials.

## Required tests
