#include <jsxer.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

std::string readFile(const std::string& path) {
    std::ifstream t(path, std::ios::binary);
    if (!t.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

int main() {
    // Read the jsxbin file
    std::string jsxbinPath = "../tests/data/jsxbin/test_number_member.jsxbin";
    std::string jsxbin = readFile(jsxbinPath);
    if (jsxbin.empty()) {
        jsxbinPath = "C:/Users/BetaTest/Downloads/fix_jsxer/tests/data/jsxbin/test_number_member.jsxbin";
        jsxbin = readFile(jsxbinPath);
        if (jsxbin.empty()) {
            std::cerr << "Could not read jsxbin file." << std::endl;
            return 1;
        }
    }

    std::string decompiled;
    int err = jsxer::decompile(jsxbin.c_str(), decompiled);

    if (err != 0) {
        std::cerr << "Decompilation failed with error code: " << err << std::endl;
        return 1;
    }

    std::cout << "Decompiled output:" << std::endl;
    std::cout << decompiled << std::endl;

    // Check for invalid syntax patterns
    if (decompiled.find("60.toString()") != std::string::npos ||
        decompiled.find("0.toString()") != std::string::npos ||
        decompiled.find("30.toString()") != std::string::npos ||
        decompiled.find("20.toString()") != std::string::npos) {
        std::cerr << "ERROR: Found invalid syntax like '60.toString()'" << std::endl;
        std::cerr << "Expected: '(60).toString()'" << std::endl;
        return 1;
    }

    // Check for valid syntax patterns
    if (decompiled.find("(60).toString()") != std::string::npos &&
        decompiled.find("(0).toString()") != std::string::npos) {
        std::cout << "SUCCESS: All number literals are correctly wrapped in parentheses" << std::endl;
        return 0;
    }

    std::cerr << "WARNING: Expected patterns not found in output" << std::endl;
    return 1;
}
