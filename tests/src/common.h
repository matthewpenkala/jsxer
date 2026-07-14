#pragma once

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <jsxer.h>

namespace jsxer::test {
inline std::filesystem::path data_path(const std::string& category, const std::string& filename) {
    return std::filesystem::path(JSXER_TEST_DATA_DIR) / category / filename;
}

inline std::string read_file(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open test fixture: " + path.string());
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

inline std::string strip_generated_header(const std::string& value) {
    const size_t marker = value.find("Decompiled with Jsxer");
    if (value.rfind("/*", 0) != 0 || marker == std::string::npos || marker > 64) {
        return value;
    }

    const size_t header_end = value.find("*/");
    return header_end == std::string::npos ? value : value.substr(header_end + 2);
}

inline std::string trim(std::string value) {
    const auto not_space = [](unsigned char character) {
        return !std::isspace(character);
    };

    value.erase(value.begin(), std::find_if(value.begin(), value.end(), not_space));
    value.erase(std::find_if(value.rbegin(), value.rend(), not_space).base(), value.end());
    return value;
}

inline std::string normalize_lines(std::string value) {
    value.erase(std::remove(value.begin(), value.end(), '\r'), value.end());
    return trim(std::move(value));
}

inline std::string remove_whitespace(const std::string& value) {
    std::string result;
    for (const unsigned char character : value) {
        if (!std::isspace(character)) {
            result += static_cast<char>(character);
        }
    }
    return result;
}

inline bool require(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << message << std::endl;
    }
    return condition;
}

inline int decompile(const std::string& compiled, std::string& decompiled, const std::string& source = "") {
    const int result = jsxer::decompile_test(compiled, decompiled);

    if (!source.empty()) {
        std::cout << "--------------------- Source ---------------------\n"
                  << source << '\n';
    }

    std::cout << "------------------- Decompiled -------------------\n"
              << decompiled << "\n--------------------------------------------------\n";
    return result;
}
} // namespace jsxer::test
