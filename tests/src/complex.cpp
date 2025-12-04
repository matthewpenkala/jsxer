#include <jsxer.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>

const char compiled[] = "@JSXBIN@ES@2.0@MyBbyBn0ADJAnASzBjFByBnd2iNOftJBnASzBjOCyBncfftJEnASzBjSDyBCzBhLE"
"CECECECEdVCfyBFeBhNFeAdCzBhcFEXzFjGjMjPjPjSGfjzEiNjBjUjIHfRBCzBhPIVBfyBnnd2QOff"
"nndKCEnEXGfjHfRBCIVBfyBnnd2QOffeBhQnEXzIjUjPiTjUjSjJjOjHJfEXGfjHfRBCIVBfyBnnd2Q"
"OffnfnnnneBhadCFCzBhFKEXGfjHfRBCIVBfyBnndhcffnndhcnndKCEnCKEXGfjHfRBCIVBfyBnndh"
"cffnndhceBhQnEXJfCKEXGfjHfRBCIVBfyBnndhcffnndhcnfnnnneBhadCFEXGfjHfRBCKVBfyBnnd"
"hcffnndKCEnEXGfjHfRBCKVBfyBnndhcffeBhQnEXJfEXGfjHfRBCKVBfyBnndhcffnfnnnffADB40B"
"iAC4B0AiAD4C0AiAADAzALByB";

std::string readFile(const std::string& path) {
    std::ifstream t(path);
    if (!t.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

std::string normalize(std::string s) {
    s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());
    // Trim trailing whitespace
    while (!s.empty() && std::isspace(s.back())) {
        s.pop_back();
    }
    // Trim leading whitespace
    while (!s.empty() && std::isspace(s.front())) {
        s.erase(s.begin());
    }
    return s;
}

int main() {
    std::string decompiled;
    int err = jsxer::decompile(compiled, decompiled);

    if (err != 0) {
        std::cerr << "Decompilation failed with error code: " << err << std::endl;
        return err;
    }

    // Assuming running from build directory, so path is ../tests/data/jsx/complex_verified.jsx
    std::string expectedPath = "../tests/data/jsx/complex_verified.jsx";
    std::string expected = readFile(expectedPath);
    if (expected.empty()) {
        // Try absolute path if relative fails (fallback)
        expectedPath = "C:/Users/BetaTest/Downloads/fix_jsxer/tests/data/jsx/complex_verified.jsx";
        expected = readFile(expectedPath);
        if (expected.empty()) {
             std::cerr << "Could not read expected file." << std::endl;
             return 1;
        }
    }

    if (normalize(decompiled) != normalize(expected)) {
        std::cerr << "Decompiled code does not match expected output." << std::endl;
        std::cerr << "Expected:\n" << expected << "\n";
        std::cerr << "Actual:\n" << decompiled << "\n";
        return 1;
    }

    printf("--------------------------------------------------\n");
    printf("%s\n", decompiled.c_str());
    printf("--------------------------------------------------\n");
    printf("Test Passed: Decompiled code matches expected output.\n");

    return 0;
}
