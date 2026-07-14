#include "BinaryExpression.h"

#include <cstring>

namespace jsxer::nodes {
    string BinaryExpression::create_expr(const string &literal, const AstOpNode& exprNode) {
        bool parenthesis = false;
        string expression;

        if (exprNode != nullptr && exprNode->type() == NodeType::BinaryExpression) {
            auto binExpr = std::dynamic_pointer_cast<BinaryExpression>(exprNode);
            expression = binExpr->get_op();

            bool associative = (strcmp(binExpr->get_op_name().c_str(), "*") == 0
                                && strcmp(op_name.c_str(), "*") == 0) ||
                               (strcmp(binExpr->get_op_name().c_str(), "+") == 0
                                && strcmp(op_name.c_str(), "+") == 0);

            parenthesis = !associative;
        } else if (exprNode != nullptr && (exprNode->type() == NodeType::LocalAssignmentExpression || exprNode->type() == NodeType::AssignmentExpression || exprNode->type() == NodeType::TernaryExpression)){
            parenthesis = true;
            expression = exprNode->to_string();
        } else {
            expression = exprNode == nullptr ? literal : exprNode->to_string();
            if (exprNode != nullptr && exprNode->type() == NodeType::CallExpression && expression.find('?') != string::npos && expression.find(':') != string::npos) {
                if (expression.find("var ") != 0 && expression.find("function") != 0) {
                    parenthesis = true;
                }
            }
        }

        return parenthesis ? "(" + expression + ")" : expression;
    }

    void BinaryExpression::parse() {
        op_name = decoders::d_operator(reader);
        left = decoders::d_node(reader);
        right = decoders::d_node(reader);
        literalLeft = decoders::d_variant(reader);
        literalRight = decoders::d_variant(reader);

        string leftExp = create_expr(literalLeft, left);
        string rightExp = create_expr(literalRight, right);

        // CRITICAL FIX: Wrap numeric BinaryExpression when used in string concatenation
        // Pattern: "string" + (num1 + num2)
        // If left is a string and right is a numeric binary expression, we MUST wrap right in parens
        if (op_name == "+") {
            bool leftIsString = leftExp.find('"') != string::npos || leftExp.find("'") != string::npos;
            
            if (leftIsString && right != nullptr && right->type() == NodeType::BinaryExpression) {
                // Check if right expression is purely numeric (no strings)
                if (rightExp.find('"') == string::npos && rightExp.find("'") == string::npos) {
                     // Check if it's an additive operation that needs protection
                     // We cast to check op_name, or just assume + needs wrapping in string context
                     // Since we verified it's a BinaryExpression and has no strings, wrapping is safe
                     
                     // Avoid double wrapping if create_expr already wrapped it (e.g. due to precedence)
                     if (rightExp.length() < 2 || rightExp.front() != '(' || rightExp.back() != ')') {
                        rightExp = "(" + rightExp + ")";
                     }
                }
            }
        }

        if ((!leftExp.empty() && rightExp.empty()) || (leftExp.empty() && !rightExp.empty())) {
            op = leftExp + rightExp;
        } else {
            op = leftExp + ' ' + op_name + ' ' + rightExp;
        }
    }

    string BinaryExpression::to_string() {
        return op;
    }
}
