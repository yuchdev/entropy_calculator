# Security

Threat models, security review outputs, and posture documentation for Encrypt Detector.

The `security-auditor` agent owns this directory. Every change touching auth,
secrets, external integrations, or untrusted-input ingestion triggers a security
review whose output is stored here.

## Naming convention

`threat-model-<scope>.md` for threat models, `review-<scope>-<YYYY-MM-DD>.md`
for point-in-time reviews.

## What a threat model must contain

1. **Scope** - which components and trust boundaries are in scope.
2. **Assets** - what secrets, PII, and data are handled.
3. **Threat actors** - attacker profiles considered.
4. **STRIDE analysis** - Spoofing, Tampering, Repudiation, Info Disclosure, DoS, Elevation.
5. **Mitigations** - existing controls and open gaps.
6. **Verdict** - CRITICAL (merge blocked) / HIGH / MEDIUM / LOW / INFO.

## Security rules (non-negotiable)

- Never log secrets; rely on this project's log-redaction mechanism (if any)
  and verify it covers new sinks.
- Never hard-code credentials. Read from settings/env.
- Treat all untrusted external input as sensitive - no unredacted raw input
  in logs, exceptions, stored reports, or API error bodies.
- Untrusted input must never reach a shell, SQL string, `eval`, or an AI
  prompt without sanitisation/parameterisation.

> **SME REVIEW NEEDED (AI-drafted - verify before relying on this):**
>
> ## Threat model - file and sequence entropy scan (as of commit `0ec911b`)
>
> **Scope.** Two components are in scope.
> - The `entropy` library reads a file (`ShannonEncryptionChecker::get_file_entropy` → `read_file_probabilities`) or a memory buffer (`get_sequence_entropy`) and returns entropy plus a `Plain`/`Binary`/`Encrypted` verdict.
> - The `entropy_calculator` CLI takes options from Boost.Program_options and prints results to `stdout`.
>
> There is no network I/O, authentication, persistence, logging framework or credential handling. The trust boundary is the **content and name of the scanned file**, plus the numeric CLI arguments. Future scope from `docs/roadmap/` adds stdin data (M4.4), directory recursion with symlink following and `--files-from -` (M4.1), magic-byte parsing (M3.3), JSON/CSV output to downstream pipelines and SIEMs (M4.2), and a thread pool (M4.3).
>
> **Assets.**
> - The integrity of the reported entropy and verdict. Users rely on it to decide whether data is encrypted, for example in ransomware triage or dataset labelling.
> - The availability of the host running the scan.
> - Read-only access to whatever files the invoking user can read.
>
> No secrets or PII are stored or transmitted. File contents are reduced to a 256-bin histogram, and only the path is echoed.
>
> **Threat actors.**
> - (1) The author of a scanned file, such as a malware or ransomware author, who wants the file misclassified or wants to crash or hang the scanner.
> - (2) A local user or script supplying CLI arguments, who has the same trust as the process.
> - (3) Future: a party controlling file names or directory layout in a batch-scanned tree, or a downstream consumer parsing the output.
>
> **STRIDE.**
> - **Spoofing / evasion (verdict integrity):**
>   - Whole-file entropy is an average, so an attacker can pad an encrypted payload with low-entropy bytes and get `Plain` or `Binary`.
>   - Compressed archives and media score near 8.0, so they are reported as `Encrypted` (acknowledged in `m3_classification.md`).
>   - The thresholds in `estimated_epsilon` and the fixed `> 6.0` Binary cut-off are hand-tuned, with no statistical basis.
>   - Mitigations are planned but not implemented: block entropy (M2), the statistical battery and magic bytes (M3).
> - **Tampering (input during scan):**
>   - `fs::file_size` is sampled before the read loop, and probabilities are divided by that size. A file that grows or shrinks mid-scan yields probabilities not summing to 1, so the entropy is wrong.
>   - The loop tests `eof()` before `read()` and counts one stale byte.
>   - A Ctrl+C interrupt returns an empty probability vector, which the kernel turns into entropy `0.0`. The run then reports it as `Plain` instead of "cancelled".
> - **Repudiation:** not applicable. There are no user actions to attribute.
> - **Information disclosure:** low.
>   - The file path is printed verbatim to `stdout` (`calculate_file_entropy` in `main.cpp`), so control characters or terminal escape sequences in a file name reach the terminal.
>   - In M4, the same names reach JSON/CSV output, so correct escaping becomes a requirement (M4.2 tests).
> - **Denial of service:**
>   - `-s/--sequence-size` directly sizes a `std::vector<uint8_t>` (`random_distributions.cpp`). A huge value causes `bad_alloc`, which is caught by the generic handler.
>   - Scans read one byte per `read` call with a per-byte callback, so a very large file or an endless device or FIFO path ties the process up. On POSIX there is no cancel path, because only Windows installs a console handler.
>   - Planned symlink recursion needs the visited-inode guard specified in M4.1.
> - **Elevation / memory safety:**
>   - Histogram indexing uses a `uint8_t` into 256-element vectors, which is in bounds.
>   - `generate_normal_distribution` casts `gen() * 255` (a float that may be negative or above 255 with the default mean 0 and stddev 1) to `uint8_t`. That is undefined behaviour for out-of-range floating-to-integer conversion.
>   - `interrupt_all_` is a non-atomic `static bool` written from the Windows console-handler thread while the scan thread reads it, which is a data race.
>   - `uint8_codecvt.h` specialises `std::char_traits<uint8_t>` (not a program-defined type) and defines non-inline symbols in a header.
>   - The checker constructor mutates the process-wide locale through `std::locale::global`.
>
> **Error handling.** `main.cpp` catches `std::exception`, prints `"General exception: ..."` and returns `EXIT_SUCCESS`. A missing or unreadable file therefore exits 0, and scripts cannot detect failure. M4.1 plans exit code 2 for per-file failures.
>
> **Dependencies.** Boost >= 1.61 is linked statically (Program_options, Filesystem, the legacy `boost/progress.hpp`). The pin dates from 2016, and the code does not build on Boost 1.72+ until M0.4. No CI or dependency audit exists yet.
>
> **Verdict (draft):** MEDIUM. There is no remote attack surface and no secret handling. The main risks are attacker-steerable misclassification and silent wrong results (stale byte, size race, interrupt reported as `0.0`, exit 0 on error). Two undefined-behaviour sites (the float-to-`uint8_t` cast and the non-atomic interrupt flag) should be fixed in M0. Re-assess when M3.3 (parsing attacker-controlled headers) and M4 (recursion, stdin, machine output, threads) land.
