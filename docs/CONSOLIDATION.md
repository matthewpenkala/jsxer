# Consolidation audit

This document records how the maintained fork was assembled. The audit was performed on 2026-07-14 so future updates can distinguish reviewed history from accidental omission.

## Baseline

- Original upstream: `AngeloD2022/jsxer` `master` at `0340bd3321be529cf775215cfac860cf2cf8b477`.
- Patch upstream: `SiliconMoss/jsxer` `master` at `0fbc70eb9144640259a7856cfead91c49823ac46`.
- SiliconMoss is a direct fork exactly two commits ahead of the original master. Both commits are retained with their original authorship: `fb29f06` and `0fbc70e`.

## Fork-network review

All 58 forks visible through the original repository's fork network were compared to `AngeloD2022/jsxer:master`, not merely sorted by recent activity.

Only two default branches contained commits ahead of upstream:

- `SiliconMoss/jsxer`: two source/test commits; included.
- `gontis/jsxer`: one commit containing generated CMake caches, object files, downloaded dependency build trees, and other local build artifacts; excluded because it contains no source fix and would pollute the repository.

Recent fork update timestamps without ahead commits were not treated as code changes.

## Original upstream branches

| Branch | Decision | Reason |
| --- | --- | --- |
| `master` | Included | Canonical C++ history and all merged releases through 1.7.4. |
| `feat/nested-jsxbin` | Already included | Its commits were merged into `master`. |
| `fix/deob-ident-overflow` | Already included | Its commits were merged into `master`. |
| `fix/reader_data_lengthcheck` | Already included | Its commits were merged into `master`. |
| `unjsxblind` | Excluded | No tree difference from its merge base; topology only. |
| `boolfix` | Not merged | Old empirical logical-precedence experiment remains disputed and unverified in upstream issue #41. Its unrelated ternary typo is superseded by SiliconMoss's tested fix. |
| `emscripten` | Not merged | 2022 work in progress, 143 commits behind, with no completed or tested WebAssembly integration. |
| `rust-rewrite` | Not merged | Explicitly incomplete replacement with only a partial AST/CLI skeleton. It is preserved upstream as research, not production code. |

## Closed, unmerged pull requests

- **#97, sparse arrays:** the narrow null-child preservation was ported and covered by a new generated fixture. Unrelated stale branch content was not copied.
- **#98, AST printer:** excluded because review identified a circular dependency and the draft was never completed.
- **#56, smart pointers:** the useful ownership conversion was superseded by later merged work already present on `master`.
- **#52, segmentation fault:** explicitly superseded upstream by later PR #63.
- **#48, backslash preprocessing:** the behavior is already present on `master`; the PR's bundled `boolfix` experiment was not imported.
- **#33, #24, #7, #6, #1:** superseded, accidentally based, or replaced by later merged changes.

## Additional maintained-fork work

The consolidation audit also examined current open bug reports and the live code paths they implicated. Version 1.8.0 adds targeted fixes for Unicode identifier corruption, unsafe empty/malformed input handling, sparse arrays, variant-storage undefined behavior, strict-aliasing violations, XML attribute bounds, CLI error suppression, C ABI validation, dependency reproducibility, current runner support, and regression-test portability.

Feature requests such as a complete JSXBIN compiler, WebAssembly UI, AST export, batch mode, and the unfinished Rust rewrite are not represented as completed fixes. They remain separate product work rather than being silently folded into a maintenance release.
