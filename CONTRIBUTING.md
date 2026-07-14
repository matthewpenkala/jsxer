# Contributing

Decompiler changes can silently produce valid-looking but incorrect code. Every parser or printer fix must therefore include a focused JSX source fixture, its compiled JSXBIN counterpart, and an assertion against the expected semantic structure.

## Local workflow

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DJSXER_WARNINGS_AS_ERRORS=ON
cmake --build build --config Debug --parallel
ctest --test-dir build -C Debug --output-on-failure
```

Keep builds outside the source tree. Before opening a pull request, also verify a Release build when your change affects CMake, packaging, exported symbols, or compiler compatibility.

## Fixtures

- Put readable inputs in `tests/data/jsx/`.
- Put matching compiled payloads in `tests/data/jsxbin/`.
- Put test executables in `tests/src/` and register them in `CMakeLists.txt`.
- Never submit proprietary or confidential scripts. Reduce reports to the smallest independently shareable reproduction.

The optional compiler helper under `tools/test-runner.js/` depends on Adobe ExtendScript components and only works on supported Windows/macOS environments. Committed JSXBIN fixtures make normal builds independent of that tool.

## Pull requests

Explain:

- the malformed output, crash, or build failure being fixed;
- why the new output preserves ExtendScript semantics;
- the regression test that fails before the change and passes afterward;
- any compatibility or security implications.

Keep unrelated refactors separate from parser fixes. New dependencies require an immutable source reference, an integrity hash, and a clear reason.
