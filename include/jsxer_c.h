#pragma once

#include <stdbool.h>
#include <stddef.h>

#if defined(_WIN32)
    #if defined(JSXER_DLL_EXPORTS)
        #define JSXER_PUBLIC_API __declspec(dllexport)
    #else
        #define JSXER_PUBLIC_API __declspec(dllimport)
    #endif
#elif defined(__GNUC__) || defined(__clang__)
    #define JSXER_PUBLIC_API __attribute__((visibility("default")))
#else
    #define JSXER_PUBLIC_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum jsxer_result {
    JSXER_RESULT_OK = 0,
    JSXER_RESULT_BUFFER_TOO_SMALL = 1,
    JSXER_RESULT_INVALID_ARGUMENT = -1,
    JSXER_RESULT_INVALID_SIGNATURE = -3,
    JSXER_RESULT_DECODE_ERROR = -4,
};

/*
 * Decompile a JSXBIN payload.
 *
 * If output is NULL, out_len receives the required text length excluding the
 * trailing NUL. If output is non-NULL, *out_len is its capacity (including
 * space for the NUL) and receives the written text length on success.
 */
JSXER_PUBLIC_API int jsxer_decompile(
    const char* input,
    size_t in_len,
    char* output,
    size_t* out_len,
    bool unblind
);

/* Backward-compatible symbol retained for existing ctypes/C++ consumers. */
JSXER_PUBLIC_API int decompile(
    const char* input,
    size_t in_len,
    char* output,
    size_t* out_len,
    bool unblind
#ifdef __cplusplus
    = false
#endif
);

#ifdef __cplusplus
}
#endif
