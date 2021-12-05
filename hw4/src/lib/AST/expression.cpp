#include "AST/expression.hpp"

ExpressionNode::ExpressionNode(const uint32_t line, const uint32_t col)
    : AstNode{line, col} {
        valid = 1;
    }
    
void ExpressionNode::setType(std::string p_type) {
    type = p_type;
}
