#pragma once

#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>

namespace utils {
inline std::string read_file_contents(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Unable to open input file: " + path.string());
    }

    return {
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    };
}

inline void write_file_contents(const std::filesystem::path& path, const std::string& contents) {
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    if (!file) {
        throw std::runtime_error("Unable to open output file: " + path.string());
    }

    file.write(contents.data(), static_cast<std::streamsize>(contents.size()));
    if (!file) {
        throw std::runtime_error("Unable to write output file: " + path.string());
    }
}
} // namespace utils
