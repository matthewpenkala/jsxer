#include "library.h"

#include <jsxer.h>

#include <cstring>
#include <string>

namespace {
int decompile_impl(const char* input, size_t in_len, char* output, size_t* out_len, bool unblind) {
    if (out_len == nullptr || (input == nullptr && in_len != 0)) {
        return JSXER_RESULT_INVALID_ARGUMENT;
    }

    const std::string compiled(input == nullptr ? "" : input, in_len);
    std::string decompiled;

    const int result = jsxer::decompile(compiled, decompiled, unblind);
    if (result != JSXER_RESULT_OK) {
        if (output != nullptr && *out_len > 0) {
            output[0] = '\0';
        }
        *out_len = 0;
        return result;
    }

    const size_t text_length = decompiled.size();
    if (output == nullptr) {
        *out_len = text_length;
        return JSXER_RESULT_OK;
    }

    if (*out_len <= text_length) {
        *out_len = text_length;
        return JSXER_RESULT_BUFFER_TOO_SMALL;
    }

    std::memcpy(output, decompiled.c_str(), text_length + 1);
    *out_len = text_length;
    return JSXER_RESULT_OK;
}
} // namespace

int jsxer_decompile(const char* input, size_t in_len, char* output, size_t* out_len, bool unblind) {
    return decompile_impl(input, in_len, output, out_len, unblind);
}

int decompile(const char* input, size_t in_len, char* output, size_t* out_len, bool unblind) {
    return decompile_impl(input, in_len, output, out_len, unblind);
}
