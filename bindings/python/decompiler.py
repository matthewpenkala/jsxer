import logging
import os
import platform
from ctypes import CDLL, POINTER, byref, c_bool, c_char_p, c_int, c_size_t, create_string_buffer
from pathlib import Path

logger = logging.getLogger(__name__)


def _library_filename():
    system = platform.system().lower()
    if system == "windows":
        return "lib-jsxer.dll"
    if system == "linux":
        return "lib-jsxer.so"
    if system == "darwin":
        return "lib-jsxer.dylib"
    raise OSError(f"Unsupported platform: {system}")


def _get_binding_path():
    explicit_path = os.environ.get("JSXER_LIBRARY")
    if explicit_path:
        candidate = Path(explicit_path).expanduser().resolve()
        if candidate.is_file():
            return candidate
        raise FileNotFoundError(f"JSXER_LIBRARY does not point to a file: {candidate}")

    repository_root = Path(__file__).resolve().parents[2]
    filename = _library_filename()
    candidates = [
        repository_root / "build" / "bin" / "Release" / filename,
        repository_root / "build" / "bin" / "Debug" / filename,
        repository_root / "bin" / "release" / "dll" / filename,
        repository_root / "bin" / "debug" / "dll" / filename,
    ]

    for candidate in candidates:
        logger.debug("Searching for Jsxer library at %s", candidate)
        if candidate.is_file():
            return candidate

    searched = "\n".join(f"  - {candidate}" for candidate in candidates)
    raise FileNotFoundError(
        "Jsxer shared library was not found. Set JSXER_LIBRARY or build the "
        f"project first. Searched:\n{searched}"
    )


_backend = CDLL(str(_get_binding_path()))
try:
    _decompile = _backend.jsxer_decompile
except AttributeError:
    # Compatibility with Jsxer releases before 1.8.0.
    _decompile = _backend.decompile

_decompile.argtypes = [c_char_p, c_size_t, c_char_p, POINTER(c_size_t), c_bool]
_decompile.restype = c_int


def decompile(compiled: str, unblind: bool = False) -> str:
    encoded = compiled.encode("utf-8")
    text_length = c_size_t(0)

    result = _decompile(
        encoded,
        len(encoded),
        None,
        byref(text_length),
        c_bool(unblind),
    )
    if result != 0:
        raise RuntimeError(f"Jsxer decompilation failed with code {result}")

    output = create_string_buffer(text_length.value + 1)
    capacity = c_size_t(len(output))
    result = _decompile(
        encoded,
        len(encoded),
        output,
        byref(capacity),
        c_bool(unblind),
    )
    if result != 0:
        raise RuntimeError(f"Jsxer decompilation failed with code {result}")

    return output.value.decode("utf-8")


if __name__ == "__main__":
    print(
        decompile(
            "@JSXBIN@ES@2.0@MyBbyBn0ACJAnABjzBjYBfneB2nfnffJBnAEXzIjUjPiTjUjSjJjOjHCfEXzKjDjI"
            "jBjSiDjPjEjFiBjUDfjBfRBFdAffRBFdQff0DzAEByB"
        )
    )
