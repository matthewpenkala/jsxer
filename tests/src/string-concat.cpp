#include "common.h"

#include <vector>

int main() {
    const std::string compiled = jsxer::test::read_file(
        jsxer::test::data_path("jsxbin", "string_concat.jsxbin")
    );

    std::string decompiled;
    if (jsxer::decompile_test(compiled, decompiled) != 0) {
        std::cerr << "String-concatenation fixture failed to decompile." << std::endl;
        return 1;
    }

    const std::string normalized = jsxer::test::remove_whitespace(decompiled);
    const std::vector<std::string> expected_patterns = {
        "\"Test:\"+(speedTime+holdTime)",
        "\"Test:\"+(speedTime+holdTime+tempFadeTime)",
        "\"Test:\"+((speedTime+holdTime)-tempFadeTime)",
        "\"linear(_t_,\"+speedTime+holdTime+(speedTime-tempFadeTime)+\",\"+(speedTime*2)+holdTime+\",-100,0)\"",
    };

    for (const auto& pattern : expected_patterns) {
        if (!jsxer::test::require(
            normalized.find(jsxer::test::remove_whitespace(pattern)) != std::string::npos,
            "Missing expected string-concatenation pattern: " + pattern + "\n" + decompiled
        )) {
            return 1;
        }
    }

    return 0;
}
