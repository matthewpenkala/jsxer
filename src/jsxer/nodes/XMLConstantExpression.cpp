#include "XMLConstantExpression.h"

namespace jsxer::nodes {
    void XMLConstantExpression::parse() {
        size_t length = decoders::d_length(reader);
        if (reader.error() != ParseError::None || !reader.claim_work(length)) {
            return;
        }

        for (size_t i = 0; i < length; ++i) {
            AstOpNode child_node = decoders::d_node(reader);
            size_t child_length = decoders::d_length(reader);
            children.emplace_back(child_node, child_length);
        }
    }

    string XMLConstantExpression::to_string() {
        static const int TYPE_NORMAL = 0;

        string result;

        std::vector<AstOpNode> normals;
        std::vector<AstOpNode> placeholders;

        for (const auto &child: children){
            if (child.second == TYPE_NORMAL){
                normals.push_back(child.first);
                continue;
            }
            placeholders.push_back(child.first);
        }

        if (normals.size() != placeholders.size() + 1)
            return "// Jsxer: XMLConstantExpression syntax recovery failed.";

        for (size_t i = 0; i < children.size(); ++i) {
            if (!(i & 1)) {
                AstOpNode normal = normals[i / 2];
                if (normal == nullptr) {
                    return "// Jsxer: XMLConstantExpression syntax recovery failed.";
                }

                result += utils::from_string_literal(normal->to_string());
                continue;
            }
            const AstOpNode& placeholder_node = placeholders[i / 2];
            if (placeholder_node == nullptr) {
                return "// Jsxer: XMLConstantExpression syntax recovery failed.";
            }

            string placeholder = placeholder_node->to_string();
            if (!placeholder.empty()) {
                placeholder.pop_back();
            }
            result += '{' + placeholder + '}';
        }

        return result;
    }
}
