# Entropy Calculator Roadmap

Planning and progress tracking for the `entropy` library and
`entropy_calculator` CLI, organized as a three-tier hierarchy:
**Milestone → Story → Task**.

**Scope:** evolve the tool from a single-file research utility into a
practical data-classification and encryption-detection toolkit without
rewriting the existing architecture.

## Hierarchy & vocabulary

| Tier          | Meaning                                                                                                       | Lives in                                                |
|---------------|---------------------------------------------------------------------------------------------------------------|---------------------------------------------------------|
| **Milestone** | A large, strategic initiative / development direction (e.g. the generic implementation effort).               | A folder `docs/roadmap/{NNNN}-{milestone-slug}/`.       |
| **Story**     | One deliverable unit of a milestone (e.g. "AWS Bedrock Backend"). Listed in the milestone's `## Stories` table. | A subfolder `…/{TT.t}-{story-slug}/` with a `README.md`. |
| **Task**      | An atomic, implementable spec (one file, one set of classes/tests). Listed in the story's `## Tasks` table. | A file `…/{TT.t}-{story-slug}/{NN}-{task-slug}.md`.   |

A *task* is part of a *story*; a *story* is part of a *milestone*.

## File & folder convention

```
docs/roadmap/
  README.md                              ← this index + convention
  {NNNN}-{milestone-slug}/               ← MILESTONE
    plan.md                              ← milestone spec; opens with a `## Stories` table
    status.md                            ← progress tracker (optional); `## Current status` table
    {TT.t}-{story-slug}/                 ← STORY
      README.md                          ← story spec; opens with a `## Tasks` table
      {NN}-{task-slug}.md                ← TASK spec
```

- `{NNNN}` - zero-padded milestone number (`0001`, `0002`, …).
- `{TT.t}` - story number, carried from the milestone's `## Stories` table (e.g. `03.2`, `06.1`, `08.0`).
- `{NN}` - zero-padded task order (`01`, `02`, …).
- Slugs are kebab-case.

### Heading vocabulary

- A milestone's `plan.md` lists its stories under a `## Stories` heading (table column: `Story`).
- A story's `README.md` lists its tasks under a `## Tasks` heading.

### Linking convention

When a document references another **specific** document, use an
absolute-from-repo-root Markdown link:

```
[docs/roadmap/0001-generic-implementation/plan.md](/docs/roadmap/0001-generic-implementation/plan.md)
```

- Always a leading `/` (repo root), never relative `../../` chains.
- **Template** paths (containing `{` / `}`, e.g. `` `{NN}-{task-slug}.md` ``) stay as
  backtick code spans, not links.
- Run `python scripts/check_doc_links.py docs/` (or the `/link-check` skill) to verify every
  link target and `#heading-anchor` resolves. The `doc_link_check` hook runs it on edits.

## Guiding principles

- Keep the two-target layout: `entropy` (library, no CLI dependencies) and
  `entropy_calculator` (thin CLI over the library).
- Every milestone ships on its own and leaves `master` buildable and tested.
- The byte histogram becomes the central data type; entropy, classification,
  block analysis and statistical tests are all derived from it.
- The header-only kernel `shannon_entropy(first, last)` stays unchanged as the
  mathematical core.
- No new third-party dependencies for the library. The CLI may use Boost
  components already required by the build.

## Milestones

| #    | Milestone                          | Depends on                          | Spec                                                                                                            | Status                                                                                                             |
|------|-------------------------------------|--------------------------------------|-------------------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------|
| 0001 | Stabilise                           | none                                 | [/docs/roadmap/0001-stabilise/plan.md](/docs/roadmap/0001-stabilise/plan.md)                                     | [/docs/roadmap/0001-stabilise/status.md](/docs/roadmap/0001-stabilise/status.md)                                     |
| 0002 | Histogram and streaming API         | 0001-stabilise                       | [/docs/roadmap/0002-histogram-streaming-api/plan.md](/docs/roadmap/0002-histogram-streaming-api/plan.md)         | [/docs/roadmap/0002-histogram-streaming-api/status.md](/docs/roadmap/0002-histogram-streaming-api/status.md)         |
| 0003 | Block (sliding-window) entropy      | 0002-histogram-streaming-api         | [/docs/roadmap/0003-block-entropy/plan.md](/docs/roadmap/0003-block-entropy/plan.md)                             | [/docs/roadmap/0003-block-entropy/status.md](/docs/roadmap/0003-block-entropy/status.md)                             |
| 0004 | Trustworthy classification          | 0002-histogram-streaming-api         | [/docs/roadmap/0004-trustworthy-classification/plan.md](/docs/roadmap/0004-trustworthy-classification/plan.md)   | [/docs/roadmap/0004-trustworthy-classification/status.md](/docs/roadmap/0004-trustworthy-classification/status.md)   |
| 0005 | Batch scanning and machine output   | 0003-block-entropy, 0004-trustworthy-classification | [/docs/roadmap/0005-batch-scanning-and-output/plan.md](/docs/roadmap/0005-batch-scanning-and-output/plan.md) | [/docs/roadmap/0005-batch-scanning-and-output/status.md](/docs/roadmap/0005-batch-scanning-and-output/status.md) |
| 0006 | Packaging and release               | 0005-batch-scanning-and-output       | [/docs/roadmap/0006-packaging-and-release/plan.md](/docs/roadmap/0006-packaging-and-release/plan.md)             | [/docs/roadmap/0006-packaging-and-release/status.md](/docs/roadmap/0006-packaging-and-release/status.md)             |

M3 (0003) and M4 (0004) are independent of each other and can proceed in
parallel once M2 (0002) lands.

## Cross-cutting quality gates

Applied to every milestone from 0001 onward.

- **Warnings as errors** on all three compilers.
- **Sanitizers**: one Linux CI job each with AddressSanitizer +
  UndefinedBehaviorSanitizer, and ThreadSanitizer from 0005 onward.
- **Determinism**: all tests that use randomness take a fixed seed; the seed
  is printed on failure.
- **No new library dependencies**: the `entropy` target must not gain link
  dependencies; CLI additions require a note in the milestone's `plan.md`.
- **Backward compatibility**: deprecated public symbols survive one release
  with `[[deprecated]]` and a migration note in `CHANGELOG.md`.
- **Changelog**: every story adds a line under the next version heading.

## Out of scope (for now)

- GUI or TUI front-ends.
- Entropy of non-byte alphabets (bit-level, n-gram, or token entropy). The
  `shannon_entropy` kernel supports them in principle; a future milestone
  could add n-gram histograms on top of the 0002 design.
- Full randomness test suites (NIST SP 800-22, Dieharder). The 0004 battery
  is a screening tool, not a certification tool; documentation must say so.
- Disk or process memory acquisition. The streaming API (0002) makes these
  possible as external tools feeding stdin.

## Suggested sequencing and effort

| Milestone | Rough effort | Can start after |
|-----------|--------------|-----------------|
| 0001 Stabilise | 3–5 days | now |
| 0002 Histogram and streaming API | 4–6 days | 0001 |
| 0003 Block entropy | 3–4 days | 0002 |
| 0004 Trustworthy classification | 8–12 days | 0002 (story 04.5's corpus can start during 0001) |
| 0005 Batch scanning and output | 6–8 days | 0003 and 0004 |
| 0006 Packaging and release | 4–6 days | 0005 |
