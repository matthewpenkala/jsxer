#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

#include <CLI/CLI.hpp>

#include <jsxer.h>

#include "utils.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    CLI::App cli{
        "JSXER - A fast and accurate JSXBIN decompiler.\n"
        "Written by Angelo DeLuca and contributors."
    };
    cli.set_version_flag("-v,--version", CONFIG_VERSION);

    bool unblind = false;
    std::string input;
    std::string output;

    auto* output_option = cli.add_option(
        "-o,--output",
        output,
        "Output path for the decompiled file"
    );
    cli.add_flag(
        "-b,--unblind",
        unblind,
        "Try renaming symbols obfuscated by JsxBlind (experimental)"
    );
    cli.add_option("input", input, "JSXBIN file to read")
        ->check(CLI::ExistingFile)
        ->required();

    CLI11_PARSE(cli, argc, argv);

    try {
        const fs::path input_path(input);
        const fs::path output_path = output_option->count() > 0
            ? fs::path(output)
            : input_path.parent_path() / (input_path.stem().string() + ".jsx");

        const fs::path output_parent = output_path.parent_path();
        if (!output_parent.empty()) {
            fs::create_directories(output_parent);
        }

        const std::string compiled = utils::read_file_contents(input_path);
        std::string decompiled;

        std::cout << "[i] Decompiling..." << std::endl;
        const int result = jsxer::decompile(compiled, decompiled, unblind);
        if (result != 0) {
            std::cerr << "[!] Decompilation failed with error code " << result << '.' << std::endl;
            return EXIT_FAILURE;
        }

        utils::write_file_contents(output_path, decompiled);
        std::cout << "[i] Wrote " << output_path.string() << std::endl;
        return EXIT_SUCCESS;
    } catch (const std::exception& error) {
        std::cerr << "[!] " << error.what() << std::endl;
        return EXIT_FAILURE;
    }
}
