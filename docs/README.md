# Documentation Registry

| Path | Purpose |
|------|---------|
| `docs/README.md` | This registry of documents under `docs/`. |
| `docs/roadmap/dev_roadmap.md` | Project development roadmap: guiding principles, milestones M0-M5, cross-cutting quality gates, out-of-scope items. |
| `docs/roadmap/m0_stabilise.md` | M0: fix known CLI and library defects, add the `ctest` target, replace `boost::progress_display`, CI. |
| `docs/roadmap/m1_histogram_streaming.md` | M1: public `ByteHistogram`, chunked reader with `ScanOptions` progress/cancel, benchmark target. |
| `docs/roadmap/m2_block_entropy.md` | M2: block and sliding-window entropy, region summarisation, CLI `--blocks`. |
| `docs/roadmap/m3_classification.md` | M3: principled threshold, statistical battery, magic-byte formats, `Classifier`, labelled corpus. |
| `docs/roadmap/m4_batch_output.md` | M4: directory and stdin batch scanning, JSON/JSONL/CSV output, parallel scan, stdin data. |
| `docs/roadmap/m5_packaging_release.md` | M5: installable CMake package, single-source version, C++17 baseline without Boost in the library, documentation. |
| `docs/roadmap/README.md` | Kit roadmap index and the Milestone → Story → Task folder convention used by `/implement-milestone` and `/implement-story`. |
| `docs/roadmap/0001-working-implementation/` | Kit example milestone (`plan.md`, `status.md`, sample story `01.0-hello-world-endpoint/`) illustrating the convention. It is scaffolding, not project scope. |
| `docs/adr/README.md`, `docs/adr/template.md` | Architecture Decision Record index and MADR template, used by `/adr-write`. |
| `docs/agent/tutorial.md` | How to work with the agents, skills and hooks, including the project-specific usage section. |
| `docs/agent/agents.md`, `docs/agent/skills.md`, `docs/agent/hooks.md` | Generated references for the installed agents, skills and hooks. Do not edit by hand. |
| `docs/dev/cpp_coding_standard.md` | C++ coding standard enforced by `cpp-expert`, `feature-reviewer` and `background-reviewer`. |
| `docs/test/code_test_coverage.md` | Kit coverage checklist and lcov/genhtml recipe for a CTest coverage build. |
| `docs/reviews/README.md`, `docs/reviews/example-report.md` | Location and format of `background-reviewer` reports (`YYYY-MM-DD-<topic>.md`). |
| `docs/security/README.md` | Security rules, threat-model format, and the first drafted threat model for the file-scanning surface. |
