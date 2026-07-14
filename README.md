# Jsxer

Jsxer is a fast C++17 decompiler for Adobe's legacy ExtendScript binary format (`.jsxbin`). It lifts JSXBIN 1.0, 2.0, and 2.1 payloads back to readable ExtendScript/JavaScript.

This maintained fork consolidates the latest C++ code from [AngeloD2022/jsxer](https://github.com/AngeloD2022/jsxer), the precedence and build fixes from [SiliconMoss/jsxer](https://github.com/SiliconMoss/jsxer), and additional reviewed correctness, safety, testing, packaging, and CI improvements. See [the consolidation audit](docs/CONSOLIDATION.md) for the exact branch, fork, and pull-request decisions.

> [!NOTE]
> The original repository has an experimental `rust-rewrite` branch. It is incomplete and is not used here; the production implementation remains the tested C++ codebase.

## Features

- JSXBIN 1.0, 2.0, and 2.1 decompilation
- Corrected ternary, binary, string-concatenation, `eval`, and numeric-member precedence
- Sparse array and Unicode identifier preservation
- Experimental JsxBlind symbol deobfuscation
- Command-line, C++, C-compatible shared-library, and Python `ctypes` interfaces
- Reproducible, checksum-verified dependencies
- Linux, macOS, and Windows CI on x64 and arm64, with sanitizers and CodeQL

## Build and test

Requirements:

- CMake 3.26 or newer
- A C++17 compiler
- Git and an internet connection for the first dependency download

Use an out-of-source build on every platform:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
ctest --test-dir build -C Release --output-on-failure
```

Install into a staging directory:

```bash
cmake --install build --config Release --prefix staging
```

The build produces:

- `jsxer` / `jsxer.exe` — command-line application
- `lib-jsxer` — C-compatible shared library
- `libjsxer` — static C++ library

Use `-DJSXER_BUILD_CLI=OFF`, `-DJSXER_BUILD_SHARED=OFF`, or `-DBUILD_TESTING=OFF` to omit optional targets.

## Command-line usage

The input file should contain the JSXBIN literal itself, normally beginning with `@JSXBIN@`.

```bash
jsxer input.jsxbin
jsxer input.jsxbin --output recovered.jsx
jsxer input.jsxbin --unblind
```

Run `jsxer --help` for all options. Jsxer returns a nonzero exit status and does not write an output file when decompilation fails.

## Library usage

The C++ API is declared in `include/jsxer.h`:

```cpp
#include <jsxer.h>

std::string output;
const int result = jsxer::decompile(compiled_jsxbin, output);
```

The stable C ABI is declared in `include/jsxer_c.h`. Call `jsxer_decompile` once with a null output pointer to obtain the required length, allocate that length plus one byte, then call it again to receive the NUL-terminated text. The legacy `decompile` symbol remains exported for compatibility.

The Python binding is in `bindings/python/decompiler.py`. It locates ordinary source-tree builds automatically; set `JSXER_LIBRARY` to an installed shared-library path when needed.

## Responsible use

Jsxer exists for source recovery, interoperability, education, and authorized security research. Respect copyright, license terms, and the people who create commercial scripts. Do not use it to take or redistribute code without permission.

## Credits and license

- Angelo DeLuca and all upstream Jsxer contributors
- [SiliconMoss](https://github.com/SiliconMoss) for the 1.7.4 precedence/build patch set and regression fixtures
- [psyirius](https://github.com/psyirius) for extensive upstream work, including the sparse-array approach reviewed and ported here
- Andrin Meier for the format research and original `jsxbin-to-jsx-converter`
- [codecopy](https://github.com/codecopy) for preserving a fork of that earlier project

Jsxer is licensed under the [GNU Affero General Public License v3.0](LICENSE).

Contributions are welcome; read [CONTRIBUTING.md](CONTRIBUTING.md) before submitting parser changes.

---

[![Star History Chart](https://api.star-history.com/svg?repos=matthewpenkala/jsxer&type=Date)](https://star-history.com/#matthewpenkala/jsxer&Date)

