#include "common.h"

int main() {
    const std::string compiled = jsxer::test::read_file(
        jsxer::test::data_path("jsxbin", "sparse-array.jsxbin")
    );

    std::string decompiled;
    if (jsxer::decompile_test(compiled, decompiled) != 0) {
        std::cerr << "Sparse-array fixture failed to decompile." << std::endl;
        return 1;
    }

    const std::string normalized = jsxer::test::remove_whitespace(decompiled);
    return jsxer::test::require(
        normalized.find("[0,1,/*hole*/,3]") != std::string::npos,
        "Sparse array lost its empty element.\n" + decompiled
    ) ? 0 : 1;
}
