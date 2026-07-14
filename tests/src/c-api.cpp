#include <jsxer_c.h>

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

namespace {
constexpr char compiled[] = "@JSXBIN@ES@2.0@MyBbyBn0ABJAnAARFFdBFdCFdDFdEFdFf0DzABByB";
}

int main() {
    size_t required = 0;
    int result = jsxer_decompile(compiled, std::strlen(compiled), nullptr, &required, false);
    if (result != JSXER_RESULT_OK || required == 0) {
        std::cerr << "C API size query failed." << std::endl;
        return 1;
    }

    size_t tiny_capacity = 1;
    char tiny_buffer[1] = {'x'};
    result = jsxer_decompile(
        compiled,
        std::strlen(compiled),
        tiny_buffer,
        &tiny_capacity,
        false
    );
    if (result != JSXER_RESULT_BUFFER_TOO_SMALL || tiny_capacity != required) {
        std::cerr << "C API did not report the required buffer size." << std::endl;
        return 1;
    }

    std::vector<char> output(required + 1);
    size_t capacity = output.size();
    result = jsxer_decompile(
        compiled,
        std::strlen(compiled),
        output.data(),
        &capacity,
        false
    );
    if (result != JSXER_RESULT_OK || capacity != required || output.back() != '\0') {
        std::cerr << "C API decompilation failed." << std::endl;
        return 1;
    }

    if (jsxer_decompile(nullptr, 1, nullptr, &required, false) != JSXER_RESULT_INVALID_ARGUMENT ||
        jsxer_decompile(compiled, std::strlen(compiled), nullptr, nullptr, false) != JSXER_RESULT_INVALID_ARGUMENT) {
        std::cerr << "C API invalid-argument checks failed." << std::endl;
        return 1;
    }

    return 0;
}
