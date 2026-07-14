# Changelog

## 1.8.0 - 2026-07-14

### Fixed

- Integrated SiliconMoss's ternary, binary, string-concatenation, nested-`eval`, and numeric-member precedence fixes.
- Preserved sparse array holes instead of dropping them during child decoding.
- Preserved non-ASCII identifiers as ECMAScript Unicode escapes.
- Removed undefined behavior from variant clearing and numeric bit conversion.
- Added bounds-safe reader access and clean errors for empty, truncated, and malformed input.
- Corrected empty/incomplete XML attribute validation.
- Made CLI file, decode, directory, and write failures return nonzero without producing misleading output.
- Hardened the C ABI's argument and buffer-size contract while retaining the legacy exported symbol.

### Changed

- Updated fmt to 12.2.0 and CLI11 to 2.6.2 using immutable, SHA-256-verified archives, and removed the unused plog dependency.
- Reworked CMake for out-of-source, installable, platform-native builds.
- Reworked the Python binding to support installed libraries and UTF-8 input lengths correctly.
- Added verified regression assertions instead of output-only smoke tests.
- Added Linux, macOS, and Windows CI across x64 and arm64, plus ASan, UBSan, CodeQL, release packaging, and Dependabot.

## 1.7.4-patched - 2025-12-04

- SiliconMoss patch release used as the consolidation baseline.
