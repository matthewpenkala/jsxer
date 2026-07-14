#include "common.h"

const char compiled[] = "@JSXBIN@ES@2.0@MyBbyBn0ADJAnASzBjFByBnd2iNOftJBnASzBjOCyBncfftJEnASzBjSDyBCzBhLE"
"CECECECEdVCfyBFeBhNFeAdCzBhcFEXzFjGjMjPjPjSGfjzEiNjBjUjIHfRBCzBhPIVBfyBnnd2QOff"
"nndKCEnEXGfjHfRBCIVBfyBnnd2QOffeBhQnEXzIjUjPiTjUjSjJjOjHJfEXGfjHfRBCIVBfyBnnd2Q"
"OffnfnnnneBhadCFCzBhFKEXGfjHfRBCIVBfyBnndhcffnndhcnndKCEnCKEXGfjHfRBCIVBfyBnndh"
"cffnndhceBhQnEXJfCKEXGfjHfRBCIVBfyBnndhcffnndhcnfnnnneBhadCFEXGfjHfRBCKVBfyBnnd"
"hcffnndKCEnEXGfjHfRBCKVBfyBnndhcffeBhQnEXJfEXGfjHfRBCKVBfyBnndhcffnfnnnffADB40B"
"iAC4B0AiAD4C0AiAADAzALByB";

int main() {
    std::string actual;
    const int result = jsxer::decompile(compiled, actual);
    if (result != 0) {
        std::cerr << "Decompilation failed with error code " << result << std::endl;
        return 1;
    }

    const std::string expected = jsxer::test::read_file(
        jsxer::test::data_path("jsx", "complex_verified.jsx")
    );

    const std::string normalized_actual = jsxer::test::normalize_lines(
        jsxer::test::strip_generated_header(actual)
    );
    const std::string normalized_expected = jsxer::test::normalize_lines(
        jsxer::test::strip_generated_header(expected)
    );

    if (!jsxer::test::require(
        normalized_actual == normalized_expected,
        "Complex precedence output did not match the verified fixture.\nExpected:\n" +
            normalized_expected + "\nActual:\n" + normalized_actual
    )) {
        return 1;
    }

    return 0;
}
