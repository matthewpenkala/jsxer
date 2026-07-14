#include "common.h"

int main() {
    const std::string compiled = jsxer::test::read_file(
        jsxer::test::data_path("jsxbin", "test_number_member.jsxbin")
    );

    std::string decompiled;
    if (jsxer::decompile_test(compiled, decompiled) != 0) {
        std::cerr << "Number-member fixture failed to decompile." << std::endl;
        return 1;
    }

    const std::string normalized = jsxer::test::remove_whitespace(decompiled);
    const bool valid =
        normalized.find("60.toString()") == std::string::npos &&
        normalized.find("(60).toString()") != std::string::npos &&
        normalized.find("(0).toString()") != std::string::npos &&
        normalized.find("(30).toString()") != std::string::npos &&
        normalized.find("(20).toString()") != std::string::npos;

    return jsxer::test::require(
        valid,
        "Numeric member access was not parenthesized correctly.\n" + decompiled
    ) ? 0 : 1;
}
