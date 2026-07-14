#include "common.h"

int main() {
    std::string output = "stale";

    const int empty_result = jsxer::decompile("", output);
    if (!jsxer::test::require(
        empty_result == static_cast<int>(DecompileResult::InvalidSignature) && output.empty(),
        "Empty input did not fail cleanly."
    )) {
        return 1;
    }

    const int truncated_result = jsxer::decompile("@JSXBIN@ES@2.0@", output);
    if (!jsxer::test::require(
        truncated_result == static_cast<int>(DecompileResult::DecodeError) && output.empty(),
        "Truncated input did not report a decode error."
    )) {
        return 1;
    }

    const int unknown_node_result = jsxer::decompile("@JSXBIN@ES@2.0@0", output);
    if (!jsxer::test::require(
        unknown_node_result == static_cast<int>(DecompileResult::DecodeError) && output.empty(),
        "An unknown node marker did not fail cleanly."
    )) {
        return 1;
    }

    const int malformed_depth_result = jsxer::decompile("@JSXBIN@ES@2.0@R0", output);
    if (!jsxer::test::require(
        malformed_depth_result == static_cast<int>(DecompileResult::DecodeError) && output.empty(),
        "Malformed depth encoding did not fail cleanly."
    )) {
        return 1;
    }

    const std::string compiled = jsxer::test::read_file(
        jsxer::test::data_path("jsxbin", "simple.jsxbin")
    );
    const int valid_result = jsxer::decompile(compiled, output);
    return jsxer::test::require(
        valid_result == static_cast<int>(DecompileResult::Success) &&
            output.find("* Version: 1.8.0") != std::string::npos,
        "A valid fixture failed or had the wrong generated version."
    ) ? 0 : 1;
}
