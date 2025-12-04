#include <jsxer.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

// Helper to read file content
std::string read_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return "";
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

// Helper to normalize string for comparison (remove whitespace)
std::string normalize(const std::string& s) {
    std::string result;
    for (char c : s) {
        if (!std::isspace(c)) {
            result += c;
        }
    }
    return result;
}

int main() {
    // Read the compiled JSXBIN file
    // Note: In a real scenario, we might embed this, but for now we read from the data directory
    // assuming the test is run from the build directory which is parallel to tests/
    std::string jsxbin_path = "../tests/data/jsxbin/string_concat.jsxbin";
    std::vector<char> content;
    
    std::ifstream file(jsxbin_path, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Failed to open jsxbin file: " << jsxbin_path << std::endl;
        return 1;
    }
    
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    content.resize(size);
    if (!file.read(content.data(), size)) {
        std::cerr << "Failed to read jsxbin file" << std::endl;
        return 1;
    }

    // Decompile
    std::string output;
    int err = jsxer::decompile(content.data(), output);
    if (err != 0) {
        std::cerr << "Decompilation failed with error code: " << err << std::endl;
        return 1;
    }

    // Verify specific patterns that were failing
    // Pattern 1: "Test: " + (speedTime + holdTime)
    // Should contain: "Test:"+(speedTime+holdTime)
    std::string normalized_output = normalize(output);
    
    std::vector<std::string> expected_patterns = {
        "\"Test:\"+(speedTime+holdTime)",
        "\"Test:\"+(speedTime+holdTime+tempFadeTime)",
        // Test 3: Allow extra parentheses which are safe
        "\"Test:\"+((speedTime+holdTime)-tempFadeTime)",
        // Test 4: In the source string_concat.jsx, test4 was written WITHOUT parens around speedTime+holdTime
        // So it correctly decompiles to the left-associative version.
        // This proves we don't over-wrap when the original code didn't have parens.
        // Note: (speedTime*2)+holdTime is also NOT wrapped as a group because it wasn't in the source.
        "\"linear(_t_,\"+speedTime+holdTime+(speedTime-tempFadeTime)+\",\"+(speedTime*2)+holdTime+\",-100,0)\""
    };

    bool all_passed = true;
    for (const auto& pattern : expected_patterns) {
        if (normalized_output.find(normalize(pattern)) == std::string::npos) {
            std::cerr << "FAILED: Expected pattern not found: " << pattern << std::endl;
            all_passed = false;
        } else {
            std::cout << "PASSED: Found pattern: " << pattern << std::endl;
        }
    }

    if (!all_passed) {
        std::cerr << "Full output:\n" << output << std::endl;
        return 1;
    }

    std::cout << "All string concatenation tests passed!" << std::endl;
    return 0;
}
