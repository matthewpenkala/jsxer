#include "common.h"

int main() {
    const std::string compiled = jsxer::test::read_file(
        jsxer::test::data_path("jsxbin", "unicode-identifiers.jsxbin")
    );

    std::string decompiled;
    if (jsxer::decompile_test(compiled, decompiled) != 0) {
        std::cerr << "Unicode-identifier fixture failed to decompile." << std::endl;
        return 1;
    }

    const std::string normalized = jsxer::test::remove_whitespace(decompiled);
    return jsxer::test::require(
        normalized.find("var\\u4e16\\u754c=42") != std::string::npos &&
            normalized.find("\\u4e16\\u754c;") != std::string::npos,
        "Unicode identifier was not preserved as ECMAScript escapes.\n" + decompiled
    ) ? 0 : 1;
}
