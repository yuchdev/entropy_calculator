# 01.1.01 — Add a test target

**Story:** [/docs/roadmap/0001-stabilise/01.1-add-test-target/README.md](/docs/roadmap/0001-stabilise/01.1-add-test-target/README.md)

## Goal

As a developer I want a `ctest`-driven unit-test target so that
every later change can be verified automatically.

## Work

- Add `tests/` directory with its own `CMakeLists.txt`, wired from the
  top-level file behind `option(ENTROPY_BUILD_TESTS "Build tests" ON)`.
- Use Boost.Test (already mentioned in README) or a header-only framework
  vendored into `tests/third_party/`. Decision: prefer Boost.Test since Boost is
  already required; fall back to a vendored header-only framework only if
  static Boost.Test linking proves fragile on Windows.
- Register tests with `add_test` so `ctest` runs them.
- Add a `tests/fixtures/` directory for small deterministic binary inputs
  generated at configure time or committed as files under 64 KiB.

## Success criteria

- `cmake --build . && ctest --output-on-failure` passes on macOS, Linux, Windows.
- A deliberately failing assertion makes `ctest` return non-zero.

## Required tests
