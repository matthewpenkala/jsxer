#include "jsxer.h"
#include "nodes/Program.h"
#include "reader.h"

#include <exception>
#include <string>

namespace {
void prepend_header(std::string& code, JsxbinVersion jsxbin_version, bool unblind) {
    std::string version;

    switch (jsxbin_version) {
        case JsxbinVersion::v10:
            version = "1.0";
            break;
        case JsxbinVersion::v20:
            version = "2.0";
            break;
        case JsxbinVersion::v21:
            version = "2.1";
            break;
        default:
            version = "VERSION UNKNOWN";
            break;
    }

    std::string header = "/*\n"
                         "* Decompiled with Jsxer\n"
                         "* Version: " CONFIG_VERSION "\n"
                         "* JSXBIN " + version + "\n";

    if (unblind) {
        header += "* Jsxblind Deobfuscation Enabled (EXPERIMENTAL)\n";
    }

    code = header + "*/\n\n" + code;
}

int decompile_impl(
    const std::string& input,
    std::string& output,
    bool unblind,
    bool include_header
) {
    output.clear();

    try {
        jsxer::Reader reader(input, unblind);
        if (!reader.verifySignature()) {
            return static_cast<int>(DecompileResult::InvalidSignature);
        }

        jsxer::nodes::Program ast(reader);
        ast.parse();
        if (reader.error() != jsxer::ParseError::None) {
            return static_cast<int>(DecompileResult::DecodeError);
        }

        output = ast.to_string();
        if (include_header) {
            prepend_header(output, reader.version(), unblind);
        }

        return static_cast<int>(DecompileResult::Success);
    } catch (const std::exception&) {
        output.clear();
        return static_cast<int>(DecompileResult::DecodeError);
    } catch (...) {
        output.clear();
        return static_cast<int>(DecompileResult::DecodeError);
    }
}
} // namespace

int jsxer::decompile(const std::string& input, std::string& output, bool unblind) {
    return decompile_impl(input, output, unblind, true);
}

int jsxer::decompile_test(const std::string& input, std::string& output, bool unblind) {
    return decompile_impl(input, output, unblind, false);
}
